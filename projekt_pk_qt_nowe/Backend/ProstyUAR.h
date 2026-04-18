#pragma once
#include "ModelARX.h"
#include "RegulatorPID.h"
#include "Generator.h"

class ProstyUAR {

	private:
	ARX _modelARX;
	RegulatorPID _regulatorPID;
	Generator _generator;

	double y_prev = 0.0;

    // Pamięć dla wykresów
    double u_prev = 0.0;
    double u_after_applied_borders = 0.0;
    double e_prev = 0.0;

	public:
		ProstyUAR(const ARX& model, const RegulatorPID& regulator);
        ProstyUAR(const ARX& model, const RegulatorPID& regulator, const Generator& generator);

        double simulate(double entry_signal);
		double getYPrev() const { return y_prev; }

		//ARX

		void set_ARX_parameters(const std::vector<double>& A, const std::vector<double>& B, int k = 0, double u = 0);

		void set_ARX_K(int k);
		void set_ARX_U(double u);
        void set_ARX_noise_deviation(double val) { _modelARX.setNoiseDeviation(val); }

        void set_ARX_U_borders(double top, double bottom);
        void set_ARX_Y_borders(double top, double bottom);
        void toggle_ARX_borders();

        void set_ARX_ACoefficient(double value, int index);
        void set_ARX_BCoefficient(double value, int index);
        void set_ARX_Coefficients(const std::vector<double>& A, const std::vector<double>& B) {
            _modelARX.set_Coefficients(A, B);
        }

        int get_ARX_K() const { return _modelARX.get_K(); }
        double get_ARX_noise_deviation() const { return _modelARX.getNoiseDeviation(); }
        std::vector<double> getARX_ACoefficients() const {
            return _modelARX.getACoefficients();
        };
        std::vector<double> getARX_BCoefficients() const {
            return _modelARX.getBCoefficients();
        }

        double get_ARX_Y_border_top() const { return _modelARX.getYBorderTop(); }
        double get_ARX_Y_border_bottom() const { return _modelARX.getYBorderBottom(); }
        double get_ARX_U_border_top() const { return _modelARX.getUBorderTop(); }
        double get_ARX_U_border_bottom() const { return _modelARX.getUBorderBottom(); }

        void reset_ARX_memory() {
            _modelARX.reset();
            y_prev = 0.0;
            u_prev = 0.0;
            e_prev = 0.0;
            u_after_applied_borders = 0.0;
        }

        void set_ARX_EnableBordersU(bool enabled) {
            _modelARX.setEnableU(enabled);
        }

        void set_ARX_EnableBordersY(bool enabled) {
            _modelARX.setEnableY(enabled);
        }

        bool get_ARX_EnableBordersU() const {
            return _modelARX.get_EnableU();
        }

        bool get_ARX_EnableBordersY() const {
            return _modelARX.get_EnableY();
        }

		//PID

		void set_PID_parameters(double k, double Ti = 0.0, double Td = 0.0, RegulatorPID::LiczCalk typ = RegulatorPID::LiczCalk::Wew);
        void set_PID_ProportionalGain(double k);
        void set_PID_IntegralTime(double Ti);
        void set_PID_DerivitiveTime(double Td);
        void set_PID_IntegrationMode(RegulatorPID::LiczCalk typ);

        double get_PID_ProportionalGain() const;
        double get_PID_IntegralTime() const;
        double get_PID_DerivitiveTime() const;

        double get_PID_P() const { return _regulatorPID.get_P(); }
        double get_PID_I() const { return _regulatorPID.get_I(); }
        double get_PID_D() const { return _regulatorPID.get_D(); }

        RegulatorPID::LiczCalk get_PID_IntegrationMode() const;

		void PID_reset_memory() { _regulatorPID.reset_memory(); }

		//Generator

		void set_Generator_parameters(double Amplitude, double simulationTimingTt, double realPeriodTrz, double fixedComponentS, double fillingP = 0.0);

		void set_Generator_AmplitudeA(double newAmplitude);
		void set_Generator_PeriodT(int simulationTimingTt, double realPeriodTrz);
		void set_Generator_FixedComponentS(double newFixedComponent);
        void set_Generator_SignalType(SignalType type);
        void set_Generator_Filling(double p) {_generator.setFillingP(p);}
        void set_Generator_ActivationTime(double val) { _generator.setActivationTime(val); }
        double get_Generator_ActivationTime() const { return _generator.getActivationTime(); }

        double get_Generator_Amplitude() const { return _generator.getAmplitudeA(); }
        double get_Generator_Offset() const { return _generator.getFixedComponentS(); }
        double get_Generator_Filling() const { return _generator.getFillingP(); }
        double get_Generator_RealPeriod() const { return _generator.getRealPeriodTrz(); }
        SignalType get_Generator_Type() const { return _generator.getSignalType(); }

        double calculateGenerator(double time) {
            return _generator.simulate(time);
        }

        //do wykresów
        double get_u_prev() const { return u_prev; }
        double get_e_prev() const { return e_prev; }
        double get_u_after_applied_borders() const { return u_after_applied_borders;}
};
