#include "config.h"

// --- Implementacja dla ARXConfig ---
QDataStream& operator<<(QDataStream& out, const ARXConfig& c) {
    out << c.a;
    out << c.b;
    out << c.k;
    out << c.z;
    out << c.max_u;
    out << c.min_u;
    out << c.max_y;
    out << c.min_y;
    out << c.czy_ograniczenia_aktywne;
    out << c.czy_zaklocenia_aktywne;
    return out;
}

QDataStream& operator>>(QDataStream& in, ARXConfig& c) {
    in >> c.a;
    in >> c.b;
    in >> c.k;
    in >> c.z;
    in >> c.max_u;
    in >> c.min_u;
    in >> c.max_y;
    in >> c.min_y;
    in >> c.czy_ograniczenia_aktywne;
    in >> c.czy_zaklocenia_aktywne;
    return in;
}

// --- Implementacja dla PIDConfig ---
QDataStream& operator<<(QDataStream& out, const PIDConfig& c) {
    out << c.wzmocnienie;
    out << c.stalaCalkowania;
    out << c.stalaRozniczkowania;
    // Enuma rzutujemy na typ bazowy qint32 dla pewności transportu
    out << static_cast<qint32>(c.tryb_calkowania);
    return out;
}

QDataStream& operator>>(QDataStream& in, PIDConfig& c) {
    qint32 tempTryb;
    in >> c.wzmocnienie;
    in >> c.stalaCalkowania;
    in >> c.stalaRozniczkowania;
    in >> tempTryb;
    c.tryb_calkowania = static_cast<LiczCalk>(tempTryb);
    return in;
}