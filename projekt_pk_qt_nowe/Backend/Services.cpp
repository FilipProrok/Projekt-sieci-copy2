#include <QJsonArray>
#include "Services.h"

// ==========================================
// Konstruktor i inicjalizacja
// ==========================================

Services::Services(ProstyUAR* uar, QObject* parent)
    : QObject(parent), prostyUAR(uar)
{
    currentTime = 0.0;
    currentIntervalMs = 200;

    current_w = 0.0;
    current_e = 0.0;
    current_u = 0.0;
    current_y = 0.0;

    // Timer
    simulationTimer = new QTimer(this);
    connect(simulationTimer, &QTimer::timeout, this, &Services::performSimulationStep);
}

// ==========================================
// Sterowanie symulacją
// ==========================================

void Services::startSimulation(int interval_ms) {
    if (interval_ms < 10) interval_ms = 10;
    if (interval_ms > 1000) interval_ms = 1000;

    currentIntervalMs = interval_ms;
    timeAccumulator = 0.0;
    realTimeTimer.start();
    simulationTimer->start(currentIntervalMs);
}

void Services::stopSimulation() {
    simulationTimer->stop();
}

void Services::resetSimulation() {
    stopSimulation();

    currentTime = 0.0;
    current_w = 0.0;
    current_e = 0.0;
    current_u = 0.0;
    current_y = 0.0;

    prostyUAR->PID_reset_memory();
    prostyUAR->reset_ARX_memory();

    emit simulationUpdated();
}

double Services::getSimulationFromUAR(double inputSignal) {
    return prostyUAR->simulate(inputSignal);
}

// ==========================================
// ARX
// ==========================================

void Services::set_ARX_parameters(std::vector<double> A, std::vector<double> B, int k, double u) {
    prostyUAR->set_ARX_parameters(A, B, k, u);
}

void Services::set_ARX_ACoefficient(double value, int index) { prostyUAR->set_ARX_ACoefficient(value, index); }
void Services::set_ARX_BCoefficient(double value, int index) { prostyUAR->set_ARX_BCoefficient(value, index); }

void Services::set_ARX_K(int k) { prostyUAR->set_ARX_K(k); }
void Services::set_ARX_U(double u) { prostyUAR->set_ARX_U(u); }

void Services::set_ARX_U_borders(double top, double bottom) { prostyUAR->set_ARX_U_borders(bottom, top); }
void Services::set_ARX_Y_borders(double top, double bottom) { prostyUAR->set_ARX_Y_borders(bottom, top); }

void Services::get_ARX_ParametersFromGui(std::vector<double> A, std::vector<double> B, int k, double u) {
    prostyUAR->set_ARX_parameters(A, B, k, u);
}

// ==========================================
// PID
// ==========================================

void Services::set_PID_parameters(double k, double Ti, double Td, RegulatorPID::LiczCalk typ) {
    prostyUAR->set_PID_parameters(k, Ti, Td, typ);
}

void Services::set_PID_ProportionalGain(double k) { prostyUAR->set_PID_DerivitiveTime(k); }
void Services::set_PID_IntegralTime(double Ti) { prostyUAR->set_PID_IntegralTime(Ti); }
void Services::set_PID_DerivitiveTime(double Td) { prostyUAR->set_PID_ProportionalGain(Td);}
void Services::set_PID_IntegrationMethod(RegulatorPID::LiczCalk typ) { prostyUAR->set_PID_IntegrationMode(typ); }

double Services::get_PID_ProportionalGain() const { return prostyUAR->get_PID_ProportionalGain(); }
double Services::get_PID_IntegralTime() const { return prostyUAR->get_PID_IntegralTime(); }
double Services::get_PID_DerivitiveTime() const { return prostyUAR->get_PID_DerivitiveTime(); }
RegulatorPID::LiczCalk Services::get_PID_IntegrationMethod() const { return prostyUAR->get_PID_IntegrationMode(); }

void Services::getPIDParametersFromGui(double k, double Ti, double Td) {
    prostyUAR->set_PID_parameters(k, Ti, Td);
}

// ==========================================
// Generator
// ==========================================

void Services::set_Generator_parameters(double Amplitude, double simulationTimingTt, double realPeriodTrz, double fixedComponentS, double fillingP) {
    prostyUAR->set_Generator_parameters(Amplitude, simulationTimingTt, realPeriodTrz, fixedComponentS, fillingP);
}

void Services::set_Generator_AmplitudeA(double newAmplitude) { prostyUAR->set_Generator_AmplitudeA(newAmplitude); }
void Services::set_Generator_PeriodT(int simulationTimingTt, double realPeriodTrz) { prostyUAR->set_Generator_PeriodT(simulationTimingTt, realPeriodTrz); }
void Services::set_Generator_FixedComponentS(double newFixedComponent) { prostyUAR->set_Generator_FixedComponentS(newFixedComponent); }
void Services::set_Generator_SignalType(SignalType type) { prostyUAR->set_Generator_SignalType(type); }

// ==========================================
// Zapis i Odczyt Konfiguracji
// ==========================================

void Services::saveConfiguration(const QString &fileName) {
    QJsonObject root;

    QJsonObject arx;
    QJsonArray arrA, arrB;
    for(double v : getARX_ACoefficients()) arrA.append(v);
    for(double v : getARX_BCoefficients()) arrB.append(v);

    arx["A"] = arrA;
    arx["B"] = arrB;
    arx["k"] = get_ARX_K();
    arx["noise_deviation"] = get_ARX_noise_deviation();

    QJsonObject borders;
    borders["u_top"] = get_ARX_U_border_top();
    borders["u_bottom"] = get_ARX_U_border_bottom();
    borders["y_top"] = get_ARX_Y_border_top();
    borders["y_bottom"] = get_ARX_Y_border_bottom();
    borders["u_enabled"] = get_ARX_EnableBordersU();
    borders["y_enabled"] = get_ARX_EnableBordersY();
    arx["Borders"] = borders;

    root["ARX"] = arx;

    QJsonObject pid;
    pid["Kp"] = get_PID_ProportionalGain();
    pid["Ti"] = get_PID_IntegralTime();
    pid["Td"] = get_PID_DerivitiveTime();
    pid["mode"] = static_cast<int>(get_PID_IntegrationMethod());
    root["PID"] = pid;

    QJsonObject gen;
    gen["amplitude"] = get_Generator_Amplitude();
    gen["period"] = get_Generator_RealPeriod();
    gen["offset"] = get_Generator_Offset();
    gen["filling"] = get_Generator_Filling();
    gen["type"] = static_cast<int>(get_Generator_Type());
    root["Generator"] = gen;

    fileHandler.save(root, fileName);
}

void Services::loadConfiguration(const QString &fileName) {
    QJsonObject root = fileHandler.load(fileName);
    if (root.isEmpty()) return;

    if (root.isEmpty()) return;

    if (root.contains("ARX")) {
        QJsonObject arx = root["ARX"].toObject();
        std::vector<double> A, B;
        for(auto v : arx["A"].toArray()) A.push_back(v.toDouble());
        for(auto v : arx["B"].toArray()) B.push_back(v.toDouble());

        set_ARX_Coefficients(A, B);
        set_ARX_K(arx["k"].toInt());
        set_ARX_noise_deviation(arx["noise_deviation"].toDouble());

        if (arx.contains("Borders")) {
            QJsonObject b = arx["Borders"].toObject();
            set_ARX_U_borders(b["u_top"].toDouble(), b["u_bottom"].toDouble());
            set_ARX_Y_borders(b["y_top"].toDouble(), b["y_bottom"].toDouble());
            set_ARX_EnableBordersU(b["u_enabled"].toBool());
            set_ARX_EnableBordersY(b["y_enabled"].toBool());
        }
    }

    // --- Odczyt Generatora ---
    if (root.contains("Generator")) {
        QJsonObject gen = root["Generator"].toObject();
        set_Generator_parameters(
            gen["amplitude"].toDouble(),
            currentIntervalMs,
            gen["period"].toDouble(),
            gen["offset"].toDouble(),
            gen["filling"].toDouble()
            );
        set_Generator_SignalType(static_cast<SignalType>(gen["type"].toInt()));
    }

    // --- Odczyt PID ---
    if (root.contains("PID")) {
        QJsonObject pid = root["PID"].toObject();
        set_PID_parameters(
            pid["Kp"].toDouble(),
            pid["Ti"].toDouble(),
            pid["Td"].toDouble(),
            static_cast<RegulatorPID::LiczCalk>(pid["mode"].toInt())
            );
    }

    //emity dla wykresów oraz kontrolek
    emit simulationUpdated();
    emit configurationLoaded();
}

// ==========================================
// Sloty
// ==========================================

// Krok symulacji razem z obsluga bufora czasowego
void Services::performSimulationStep() {
    double realDt = realTimeTimer.restart() / 1000.0;
    timeAccumulator += realDt;
    double simulationStep = currentIntervalMs / 1000.0;
    bool stateChanged = false;

    while (timeAccumulator >= simulationStep) {        
        //Symulowanie fizyki 'do tego momentu' korzystając ze sterowania z poprzedniego kroku
        prostyUAR->simulate(current_w);

        //Przesuniecie czasu symulacji
        currentTime += simulationStep;

        //Nowa wartość generatora, która ma sie pojawic na wykresie Ponieważ symulacja (Krok 1) użyła starego W, wynikowe Y w tym punkcie jeszcze nie zareaguje na ten skok.
        current_w = prostyUAR->calculateGenerator(currentTime);

        timeAccumulator -= simulationStep;
        stateChanged = true;
    }

    if (stateChanged) {
        // Pobieramy aktualny stan
        current_y = prostyUAR->getYPrev();
        current_e = prostyUAR->get_e_prev();
        current_u = prostyUAR->get_u_prev();
        current_u_sat = prostyUAR->get_u_after_applied_borders();

        emit simulationUpdated();
    }
}
