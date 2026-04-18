#ifndef CONFIG_H
#define CONFIG_H

#include <QDataStream>
#include <QVector>

enum class TypRamki : quint16 {
    KonfiguracjaArx = 1,
    KonfiguracjaPid = 2,
    DaneSymulacji   = 3, // Próbki u i y
    Sterowanie      = 4  // Start/Stop/Reset
};

struct ARXConfig {
    QVector<double> a; // m.in. dla testu skoku jednostkowego
    QVector<double> b;
    quint32 k = 1;
    double wariancja = 0.0;

    // Ograniczenia i nasycenia
    double max_u = 10.0;
    double min_u = -10.0;
};

struct PIDConfig {
    double Kp = 1.0;
    double Ti = 0.0;
    double Td = 0.0;
};

// Deklaracje operatorów serializacji
QDataStream& operator<<(QDataStream& out, const ARXConfig& c);
QDataStream& operator>>(QDataStream& in, ARXConfig& c);

QDataStream& operator<<(QDataStream& out, const PIDConfig& c);
QDataStream& operator>>(QDataStream& in, PIDConfig& c);

#endif // CONFIG_H