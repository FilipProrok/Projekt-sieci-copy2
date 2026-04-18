#include "ProstyUAR.h"

// ==========================================
// Konstruktory i logika główna
// ==========================================

ProstyUAR::ProstyUAR(const ARX& model, const RegulatorPID& regulator)
    : ProstyUAR(model, regulator, Generator(0.0, 0, 0.0, 0.0)) {
}

ProstyUAR::ProstyUAR(const ARX& model, const RegulatorPID& regulator, const Generator& generator)
    : _modelARX(model), _regulatorPID(regulator), _generator(generator) {
}

double ProstyUAR::simulate(double entry_signal) {
    double e = entry_signal - y_prev;
    e_prev = e;

    double u = _regulatorPID.calculate_u(e);
    u_prev = u;

    y_prev = _modelARX.simulate(u);
    u_after_applied_borders = _modelARX.getLastU(); // Dodatkowy wykres u po nałożeniu ograniczeń (nasyceniu)

    return y_prev;
}

// ==========================================
// ARX
// ==========================================

void ProstyUAR::set_ARX_parameters(const std::vector<double>& A, const std::vector<double>& B, int k, double u) {
    for (int i = 0; i < A.size(); i++) _modelARX.setACoefficient(i, A[i]);
    for (int i = 0; i < B.size(); i++) _modelARX.setBCoefficient(i, B[i]);
    _modelARX.setK(k);
    _modelARX.setU(u);
}

void ProstyUAR::set_ARX_K(int k) { _modelARX.setK(k); }
void ProstyUAR::set_ARX_U(double u) { _modelARX.setU(u); }

void ProstyUAR::set_ARX_U_borders(double top, double bottom) {
    if(top >= bottom){
        _modelARX.setUBorderTop(top);
        _modelARX.setUBorderBottom(bottom);
    }else{
        _modelARX.setUBorderTop(bottom);
        _modelARX.setUBorderBottom(top);
    }

}

void ProstyUAR::set_ARX_Y_borders(double top, double bottom) {
    if(top >= bottom){
        _modelARX.setYBorderTop(top);
        _modelARX.setYBorderBottom(bottom);
    }else{
        _modelARX.setYBorderTop(bottom);
        _modelARX.setYBorderBottom(top);
    }
}

void ProstyUAR::set_ARX_ACoefficient(double value, int index) { _modelARX.setACoefficient(value, index); }
void ProstyUAR::set_ARX_BCoefficient(double value, int index) { _modelARX.setBCoefficient(value, index); }

// ==========================================
// PID
// ==========================================

void ProstyUAR::set_PID_parameters(double k, double Ti, double Td, RegulatorPID::LiczCalk typ) {
    _regulatorPID.setProportionalGain(k);
    _regulatorPID.setIntegralTime(Ti);
    _regulatorPID.setDerivitiveTime(Td);
    _regulatorPID.setIntegrationMethod(typ);
}

void ProstyUAR::set_PID_ProportionalGain(double k) { _regulatorPID.setProportionalGain(k); }
void ProstyUAR::set_PID_IntegralTime(double Ti) { _regulatorPID.setIntegralTime(Ti); }
void ProstyUAR::set_PID_DerivitiveTime(double Td) { _regulatorPID.setDerivitiveTime(Td); }
void ProstyUAR::set_PID_IntegrationMode(RegulatorPID::LiczCalk typ) { _regulatorPID.setIntegrationMethod(typ); }

double ProstyUAR::get_PID_ProportionalGain() const { return _regulatorPID.getProportionalGain(); }
double ProstyUAR::get_PID_IntegralTime() const { return _regulatorPID.getIntegralTime(); }
double ProstyUAR::get_PID_DerivitiveTime() const { return _regulatorPID.getDerivitiveTime(); }
RegulatorPID::LiczCalk ProstyUAR::get_PID_IntegrationMode() const { return _regulatorPID.getIntegrationMethod(); }

// ==========================================
// Generator
// ==========================================

void ProstyUAR::set_Generator_parameters(double Amplitude, double simulationTimingTt, double realPeriodTrz, double fixedComponentS, double fillingP) {
    _generator.setAmplitudeA(Amplitude);
    _generator.setPeriodT(simulationTimingTt, realPeriodTrz);
    _generator.setFixedComponentS(fixedComponentS);
    _generator.setFillingP(fillingP);
}

void ProstyUAR::set_Generator_AmplitudeA(double newAmplitude) { _generator.setAmplitudeA(newAmplitude); }
void ProstyUAR::set_Generator_PeriodT(int simulationTimingTt, double realPeriodTrz) { _generator.setPeriodT(simulationTimingTt, realPeriodTrz); }
void ProstyUAR::set_Generator_FixedComponentS(double newFixedComponent) { _generator.setFixedComponentS(newFixedComponent); }
void ProstyUAR::set_Generator_SignalType(SignalType type) { _generator.setSignalType(type); }
