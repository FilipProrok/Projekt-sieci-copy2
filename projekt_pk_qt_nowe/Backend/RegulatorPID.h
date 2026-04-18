#pragma once

class RegulatorPID {
public:
    enum class LiczCalk {
        Zew, //calka liczona zewnetrznie
        Wew  //calka liczona wewnetrznie
    };

private:
    double k; //wzmocnienie
    double T_i; //czas calkowania
    double T_d; //czas rozniczkowania
    LiczCalk typ_liczenia_calki = LiczCalk::Zew;

    double suma_uchybow = 0.0; //Ti przed suma
    double suma_wazona_uchybow = 0.0; //Ti w sumie
    double previous_e = 0.0; //uchyb poprzedni

    double regulator_P(double e_i);
    double regulator_I(double e_i);
    double regulator_D(double e_i);

    double last_P = 0.0;
    double last_I = 0.0;
    double last_D = 0.0;

public:

    RegulatorPID(double k_new, double Ti_new = 0.0, double Td_new = 0.0, LiczCalk typ_liczenia_calki_new = LiczCalk::Zew);

    //Settery
    void setProportionalGain(double k_new);
    void setIntegralTime(double T_i_new);
    void setDerivitiveTime(double T_d_new);
    void setIntegrationMethod(LiczCalk newType);

    //Gettery
    double getProportionalGain() const;
    double getIntegralTime() const;
    double getDerivitiveTime() const;
    LiczCalk getIntegrationMethod() const;

    double get_P() const { return last_P; }
    double get_I() const { return last_I; }
    double get_D() const { return last_D; }


    double calculate_u(double e_i);

    void reset_memory();
};
