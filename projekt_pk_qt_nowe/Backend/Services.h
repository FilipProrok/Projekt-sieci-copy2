#pragma once
#include <vector>
#include <QObject>
#include <QTimer>
#include "ProstyUAR.h"
#include "filehandler.h"

#include <QElapsedTimer>//

class Services: public QObject {
    Q_OBJECT

private:
    ProstyUAR* prostyUAR;

    //do czasu i symulacji
    QTimer* simulationTimer;
    double currentTime;
    int currentIntervalMs;

    // Pamięć wartości do wykresów
    double current_w;
    double current_e;
    double current_u;
    double current_u_sat; //wartość u po nasyceniu (nałożeniu granic)
    double current_y;

    QElapsedTimer realTimeTimer;// do pomiaru rzeczywistego czasu
    double timeAccumulator;// bufor

    FileHandler fileHandler;

public:
    explicit Services(ProstyUAR* uar, QObject* parent = nullptr);

    //do symulacji
    void startSimulation(int interval_ms);
    void stopSimulation();
    void resetSimulation();
    bool isSimulationRunning() const { return simulationTimer->isActive(); }


    double getCurrentSimulationTime() const { return currentTime; }
    double getCurrentW() const { return current_w; }
    double getCurrentE() const { return current_e; }
    double getCurrentU() const { return current_u; }
    double getCurrentY() const { return current_y; }
    double getCurrentUSat() const { return current_u_sat; }

    double getSimulationFromUAR(double inputSignal);

    // ARX
    void set_ARX_parameters(std::vector<double> A, std::vector<double> B, int k = 0, double u = 0);
    void set_ARX_ACoefficient(double value, int index);
    void set_ARX_BCoefficient(double value, int index);

    void set_ARX_K(int k);
    void set_ARX_U(double u);
    void set_ARX_noise_deviation(double val) { prostyUAR->set_ARX_noise_deviation(val); }

    void set_ARX_U_borders(double top, double bottom);
    void set_ARX_Y_borders(double top, double bottom);

    void set_ARX_Coefficients(const std::vector<double>& A, const std::vector<double>& B) { prostyUAR->set_ARX_Coefficients(A, B); }

    void get_ARX_ParametersFromGui(std::vector<double> A, std::vector<double> B, int k = 0, double u = 0);
    std::vector<double> getARX_ACoefficients() const { return prostyUAR->getARX_ACoefficients(); };
    std::vector<double> getARX_BCoefficients() const { return prostyUAR->getARX_BCoefficients(); }

    int get_ARX_K() const { return prostyUAR->get_ARX_K(); }
    double get_ARX_noise_deviation() const { return prostyUAR->get_ARX_noise_deviation(); }

    double get_ARX_Y_border_top() const { return prostyUAR->get_ARX_Y_border_top(); }
    double get_ARX_Y_border_bottom() const { return prostyUAR->get_ARX_Y_border_bottom(); }
    double get_ARX_U_border_top() const { return prostyUAR->get_ARX_U_border_top(); }
    double get_ARX_U_border_bottom() const { return prostyUAR->get_ARX_U_border_bottom(); }

    void set_ARX_EnableBordersU(bool enabled) { prostyUAR->set_ARX_EnableBordersU(enabled); }

    void set_ARX_EnableBordersY(bool enabled) { prostyUAR->set_ARX_EnableBordersY(enabled); }

    bool get_ARX_EnableBordersU() const { return prostyUAR->get_ARX_EnableBordersU(); }
    bool get_ARX_EnableBordersY() const { return prostyUAR->get_ARX_EnableBordersY(); }

    // PID
    void set_PID_parameters(double k, double Ti = 0.0, double Td = 0.0, RegulatorPID::LiczCalk typ = RegulatorPID::LiczCalk::Wew);
    void set_PID_ProportionalGain(double k);
    void set_PID_IntegralTime(double Ti);
    void set_PID_DerivitiveTime(double Td);
    void set_PID_IntegrationMethod(RegulatorPID::LiczCalk typ);

    double get_PID_ProportionalGain() const;
    double get_PID_IntegralTime() const;
    double get_PID_DerivitiveTime() const;
    RegulatorPID::LiczCalk get_PID_IntegrationMethod() const;

    double get_PID_P() const { return prostyUAR->get_PID_P(); }
    double get_PID_I() const { return prostyUAR->get_PID_I(); }
    double get_PID_D() const { return prostyUAR->get_PID_D(); }


    void getPIDParametersFromGui(double k, double Ti = 0.0, double Td = 0.0);


    void PID_reset_memory() { prostyUAR->PID_reset_memory(); }

    // Generator
    void set_Generator_parameters(double Amplitude, double simulationTimingTt, double realPeriodTrz, double fixedComponentS, double fillingP = 0.0);
    void set_Generator_AmplitudeA(double newAmplitude);
    void set_Generator_PeriodT(int simulationTimingTt, double realPeriodTrz);
    void set_Generator_FixedComponentS(double newFixedComponent);
    void set_Generator_SignalType(SignalType type);
    void set_Generator_Filling(double p){prostyUAR->set_Generator_Filling(p);}
    void set_Generator_ActivationTime(double val) { prostyUAR->set_Generator_ActivationTime(val); }

    double get_Generator_Amplitude() const { return prostyUAR->get_Generator_Amplitude(); }
    double get_Generator_Offset() const { return prostyUAR->get_Generator_Offset(); }
    double get_Generator_Filling() const { return prostyUAR->get_Generator_Filling(); }
    double get_Generator_RealPeriod() const { return prostyUAR->get_Generator_RealPeriod(); }
    SignalType get_Generator_Type() const { return prostyUAR->get_Generator_Type(); }
    double get_Generator_ActivationTime() const { return prostyUAR->get_Generator_ActivationTime(); }

    //Zapis do pliku i odczyt konfiguracji
    void saveConfiguration(const QString &fileName);
    void loadConfiguration(const QString &fileName);

signals:
    void simulationUpdated();
    void configurationLoaded();

private slots:
    void performSimulationStep();
};
