#include "RegulatorPID.h"
#include <iostream>


double RegulatorPID::regulator_P(double e_i) {
    return k * e_i;
}

double RegulatorPID::regulator_I(double e_i) {
    if (T_i == 0.0) return 0.0;

    if (typ_liczenia_calki == LiczCalk::Wew) {
        suma_wazona_uchybow += e_i / T_i;
        return suma_wazona_uchybow;
    }
    else {
        suma_uchybow += e_i;
        return suma_uchybow / T_i;
    }
}

double RegulatorPID::regulator_D(double e_i) {
    double result = T_d * (e_i - previous_e);
    previous_e = e_i;

    return result;
}


// Konstruktor
RegulatorPID::RegulatorPID(double k_new, double Ti_new, double Td_new, LiczCalk typ_liczenia_calki_new) :
    k(k_new), T_i(Ti_new), T_d(Td_new), typ_liczenia_calki(typ_liczenia_calki_new) {}

// Settery
void RegulatorPID::setProportionalGain(double k_new) {
    k = k_new;
}

void RegulatorPID::setIntegralTime(double T_i_new) {
    T_i = T_i_new;
}

void RegulatorPID::setDerivitiveTime(double T_d_new) {
    T_d = T_d_new;
}

void RegulatorPID::setIntegrationMethod(LiczCalk newType) {
    if (typ_liczenia_calki == newType) return;

    if (newType == LiczCalk::Wew) {
        if (T_i != 0) {
            suma_wazona_uchybow = suma_uchybow / T_i;
        }
        else {
            suma_wazona_uchybow = 0.0;
        }
    }
    else {
        suma_uchybow = suma_wazona_uchybow * T_i;
    }
    typ_liczenia_calki = newType;
}

// Gettery
double RegulatorPID::getProportionalGain() const {
    return k;
}

double RegulatorPID::getIntegralTime() const {
    return T_i;
}

double RegulatorPID::getDerivitiveTime() const {
    return T_d;
}

RegulatorPID::LiczCalk RegulatorPID::getIntegrationMethod() const {
    return typ_liczenia_calki;
}

double RegulatorPID::calculate_u(double e_i) {
    last_P = regulator_P(e_i);
    last_I = regulator_I(e_i);
    last_D = regulator_D(e_i);

    return last_P + last_I + last_D;
}

void RegulatorPID::reset_memory() {
    suma_uchybow = 0.0;
    suma_wazona_uchybow = 0.0;
    previous_e = 0.0;
}
