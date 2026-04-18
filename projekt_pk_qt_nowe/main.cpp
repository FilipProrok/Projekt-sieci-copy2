#include "mainwindow.h"

#include <QApplication>

#include "Backend/ProstyUAR.h"
#include "Backend/Services.h"

#define TEST_KOMUNIKACJI

#ifndef TEST_KOMUNIKACJI

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Setup obiektow i wartosci domyslne
    ARX modelARX({ -0.4, -0.6 }, { 0.6, 0.4 }, 1, 0);
    RegulatorPID regulatorPID(0.5, 5.0, 0);
    Generator generator(1.0, 200, 10.0, 0.0, 0.5);
    ProstyUAR uar(modelARX, regulatorPID, generator);

    Services services(&uar);

    MainWindow w(&services);
    w.show();
    return a.exec();
}

#endif



#ifdef TEST_KOMUNIKACJI

#include <QTcpServer>
#include <QTcpSocket>
#include <QCoreApplication>
#include "config.h"

// --- WSPÓLNA FUNKCJA WYSYŁAJĄCA ---
template <typename T>
void wyslijKonfiguracje(QTcpSocket* socket, typ_konfiguracji typ, const T& config) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out << static_cast<quint32>(0);       // Miejsce na rozmiar
    out << static_cast<quint16>(typ);     // Typ konfiguracji
    out << config;                        // Właściwe dane

    out.device()->seek(0);                // Powrót i zapisanie rozmiaru
    out << static_cast<quint32>(block.size() - sizeof(quint32));

    socket->write(block);
    socket->flush();
}

// --- LOGIKA SERWERA (ODBIORNIKA) ---
void uruchomJakoSerwer(QCoreApplication& a) {
    QTcpServer* server = new QTcpServer(&a);
    if (!server->listen(QHostAddress::AnyIPv4, 12345)) {
        qCritical() << "[SERWER] Błąd uruchamiania:" << server->errorString();
        return;
    }
    qDebug() << "[SERWER] Oczekuję na dane na porcie:" << server->serverPort();

    QObject::connect(server, &QTcpServer::newConnection, [server]() {
        QTcpSocket* clientSocket = server->nextPendingConnection();
        qDebug() << "[SERWER] Otrzymano połączenie od instancji wysyłającej!";

        QObject::connect(clientSocket, &QTcpSocket::disconnected, clientSocket, &QObject::deleteLater);

        QObject::connect(clientSocket, &QTcpSocket::readyRead, [clientSocket]() {
            QDataStream in(clientSocket);

            while (clientSocket->bytesAvailable()>= sizeof(quint32))
            {
                in.startTransaction();

                quint32 rozmiar;
                in >> rozmiar;

                // Sprawdzamy, czy w buforze jest już cała paczka (rozmiar + reszta danych)
                if (clientSocket->bytesAvailable() < rozmiar) {
                    in.rollbackTransaction();
                    break; // PRZERWANIE PĘTLI: Czekamy na resztę danych (na kolejny readyRead)
                }

                quint16 typ;
                in >> typ;

                if (typ == ArxConfig) {
                    ARXConfig config;
                    in >> config;
                    qDebug().noquote() << "[SERWER] ---> Odebrano ARXConfig:"
                                       << "\n  Wielomiany - A:" << config.a << " B:" << config.b
                                       << "\n  Parametry  - k:" << config.k << " z:" << config.z
                                       << "\n  Limity U   - min:" << config.min_u << " max:" << config.max_u
                                       << "\n  Limity Y   - min:" << config.min_y << " max:" << config.max_y
                                       << "\n  Flagi      - Ograniczenia:" << config.czy_ograniczenia_aktywne
                                       << " Zaklocenia:" << config.czy_zaklocenia_aktywne;                }
                else if (typ == PidConfig) {
                    PIDConfig config;
                    in >> config;
                    qDebug().noquote() << "[SERWER] ---> Odebrano PIDConfig:"
                                       << "\n  Wzmocnienie (k):" << config.wzmocnienie
                                       << "\n  Stala calkowania (Ti):" << config.stalaCalkowania
                                       << "\n  Stala rozniczkowania (Td):" << config.stalaRozniczkowania
                                       << "\n  Tryb calkowania (0=Zew, 1=Wew):" << static_cast<qint32>(config.tryb_calkowania);
                }
                in.commitTransaction();
            }

        });
    });
}

// --- LOGIKA KLIENTA (NADAJNIKA) ---
void uruchomJakoKlient(QCoreApplication& a) {
    QTcpSocket* klient = new QTcpSocket(&a);
    qDebug() << "[KLIENT] Próba połączenia z instancją nasłuchującą...";

    klient->connectToHost(QHostAddress::LocalHost, 12345);

    if (klient->waitForConnected(3000)) {
        qDebug() << "[KLIENT] Połączono! Wysyłam paczki z konfiguracjami...";

        // 1. Paczka ARX
        ARXConfig testArx;
        testArx.a = {1.5, -0.7}; testArx.b = {0.2, 0.5}; testArx.k = 5;
        wyslijKonfiguracje(klient, ArxConfig, testArx);

        // 2. Paczka PID
        PIDConfig testPid;
        testPid.wzmocnienie = 42.0; testPid.tryb_calkowania = LiczCalk::Wew;
        wyslijKonfiguracje(klient, PidConfig, testPid);

        // Odczekanie na wysłanie i zamknięcie tej instancji programu
        klient->waitForBytesWritten(1000);
        qDebug() << "[KLIENT] Dane wysłane. Zamykam tę instancję.";
        QTimer::singleShot(2000, &a, &QCoreApplication::quit);
    } else {
        qCritical() << "[KLIENT] Błąd: Instancja nasłuchująca nie jest włączona.";
        QTimer::singleShot(0, &a, &QCoreApplication::quit);
    }
}

// --- GŁÓWNY PUNKT WEJŚCIA ---
int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();

    // Sprawdzamy z jaką flagą został uruchomiony program
    if (args.contains("-client")) {
        qDebug() << "=== URUCHOMIONO W TRYBIE WYSYŁANIA (KLIENT) ===";
        uruchomJakoKlient(a);
    }
    else if (args.contains("-server")) {
        qDebug() << "=== URUCHOMIONO W TRYBIE ODBIORU (SERWER) ===";
        uruchomJakoSerwer(a);
    }
    else {
        qDebug() << "Nie podano argumentu (-server lub -client). Domyślnie uruchamiam jako SERWER...";
        uruchomJakoSerwer(a);
    }

    return a.exec();
}

#endif
