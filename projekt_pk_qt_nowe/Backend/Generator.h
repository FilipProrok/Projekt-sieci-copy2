#pragma once
#include <cmath>

enum class SignalType {
    SINUSOIDAL,
    RECTANGULAR
};

class Generator {
private:
    SignalType signalType = SignalType::SINUSOIDAL;
    double amplitudeA = 0.0;
    int periodT = 1;
    double fixedComponentS = 0.0;

    double fillingP = 0.5;

    double generateSinusoidalSignal(unsigned int i);
    double generateRectangularSignal(unsigned int i);

    double activationTime = 0.0;
    int Tt = 200;

public:
    Generator(double amplitude, int simulationTimingTt, double realPeriodTrz, double fixedComponentS);
    Generator(double amplitude, int simulationTimingTt, double realPeriodTrz, double fixedComponentS, double fillingP);

    //Settery
    void setSignalType(SignalType newType);
    void setAmplitudeA(double newAmplitude);
    void setPeriodT(int simulationTimingTt, double realPeriodTrz);
    void setFixedComponentS(double newFixedComponent);
    void setFillingP(double newFillingP);
    void setActivationTime(double time) { activationTime = time; }


    //Gettery
    int getPeriodT() const { return periodT; }
    double getAmplitudeA() const { return amplitudeA; }
    double getFixedComponentS() const { return fixedComponentS; }
    double getFillingP() const { return fillingP; }
    SignalType getSignalType() const { return signalType; }
    double getRealPeriodTrz() const { return (static_cast<double>(periodT) * Tt) / 1000.0; }
    double getActivationTime() const { return activationTime; }


    //Symulacja
    double generateSignal(unsigned int i);
    double simulate(double time);
};
