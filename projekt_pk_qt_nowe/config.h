#ifndef CONFIG_H
#define CONFIG_H
#include <QDataStream>
#include <QVector>
#include <QtGlobal>
enum typ_konfiguracji : quint16
{
    ArxConfig = 1,
    PidConfig = 2
};

struct ARXConfig {
    QVector<double> a;
    QVector<double> b;
    qint32 k = 1;
    double z = 0.0;

    double max_u = 10.0;
    double min_u = -10.0;
    double max_y = 10.0;
    double min_y = -10.0;

    bool czy_ograniczenia_aktywne = true;
    bool czy_zaklocenia_aktywne = true;
};

enum class LiczCalk : qint32 { Zew = 0, Wew = 1 };

struct PIDConfig {
    double wzmocnienie = 0.0;
    double stalaCalkowania = 0.0;
    double stalaRozniczkowania = 0.0;
    LiczCalk tryb_calkowania = LiczCalk::Zew;
};

QDataStream& operator<<(QDataStream& out, const ARXConfig& c);
QDataStream& operator>>(QDataStream& in, ARXConfig& c);
QDataStream& operator<<(QDataStream& out, const PIDConfig& c);
QDataStream& operator>>(QDataStream& in, PIDConfig& c);

#endif // CONFIG_H
