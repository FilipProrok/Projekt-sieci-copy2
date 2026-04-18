#include <cmath>
#include "Generator.h"

using namespace std;

double Generator::generateSinusoidalSignal(unsigned int i) {
    return amplitudeA * sin((static_cast<double>(i % periodT) / static_cast<double>(periodT)) * 2 * 3.1415926) + fixedComponentS;
}

double Generator::generateRectangularSignal(unsigned int i) {
    // Obliczamy próg przełączenia
    int threshold = static_cast<int>(static_cast<double>(periodT) * fillingP);

    // Zabezpieczenie przed dzieleniem przez zero (choć periodT jest >= 1 z settera)
    if (periodT == 0) return fixedComponentS;

    if ((i % periodT) < threshold)
        return amplitudeA + fixedComponentS;
    else
        return fixedComponentS;
}

//KONSTRUKTORY

Generator::Generator(double amplitude, int simulationTimingTt, double realPeriodTrz, double fixedComponentS) {
    setAmplitudeA(amplitude);
    setFixedComponentS(fixedComponentS);
    setPeriodT(simulationTimingTt, realPeriodTrz);
    setFillingP(0.5);
    setSignalType(SignalType::SINUSOIDAL);
}

Generator::Generator(double amplitude, int simulationTimingTt, double realPeriodTrz, double fixedComponentS, double fillingP) {
    setAmplitudeA(amplitude);
    setFixedComponentS(fixedComponentS);
    setPeriodT(simulationTimingTt, realPeriodTrz);
    setFillingP(fillingP);
    setSignalType(SignalType::RECTANGULAR);
}

// --- SETTERY ---

void Generator::setSignalType(SignalType newType) {
    signalType = newType;
}

void Generator::setAmplitudeA(double newAmplitude) {
    if (newAmplitude < 0.0)
        newAmplitude = 0.0;
    amplitudeA = newAmplitude;
}

void Generator::setPeriodT(int simulationTimingTt, double realPeriodTrz) {
    this->Tt = simulationTimingTt;

    if (realPeriodTrz <= 0.0)
        realPeriodTrz = 1.0;
    if (simulationTimingTt <= 0)
        simulationTimingTt = 1;

    periodT = static_cast<int>(std::round(realPeriodTrz * 1000 / static_cast<double>(simulationTimingTt)));

    if (periodT < 1)
        periodT = 1;
}

void Generator::setFixedComponentS(double newFixedComponent) {
    fixedComponentS = newFixedComponent;
}

void Generator::setFillingP(double newFillingP) {
    if (newFillingP < 0.0) newFillingP = 0.0;
    if (newFillingP > 1.0) newFillingP = 1.0;
    fillingP = newFillingP;
}

//SYMULACJA

double Generator::generateSignal(unsigned int i) {
    if (signalType == SignalType::SINUSOIDAL) {
        return generateSinusoidalSignal(i);
    }
    else if (signalType == SignalType::RECTANGULAR) {
        return generateRectangularSignal(i);
    }
    return 0.0;
}

double Generator::simulate(double time) {
    if (time < activationTime) {
        return fixedComponentS;
    }

    double shiftedTime = time - activationTime;

    if (Tt <= 0) Tt = 200;

    unsigned int i = static_cast<unsigned int>((shiftedTime * 1000.0) / Tt);

    return generateSignal(i);
}
