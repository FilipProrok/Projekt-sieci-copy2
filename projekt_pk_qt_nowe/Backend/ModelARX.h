#include <vector>
#include <deque>

#include <random>
#include <chrono>

struct Borders {
	double top;
	double bottom;
};

class ARX {
private:
	Borders u = { 10.0, -10.0 }; //wejście
	Borders y = { 10.0, -10.0 }; //wyjście

	std::deque<double> uHistory;
	std::deque<double> yHistory;

    std::vector<double> ACoefficients;
	std::vector<double> BCoefficients;

	double z;
	int k;
    bool areBordersU_Enabled = true;
    bool areBordersY_Enabled = true;

    double noise_deviation = 0.0;

    std::default_random_engine generator;
    std::normal_distribution<double> distribution;

    void setY(double newY);

public:

	ARX(std::vector<double> A, std::vector<double> B, int k = 0, double u = 0);

    //Settery
	void setACoefficient(int id, double value);
	void setBCoefficient(int id, double value);
	void setK(int newDelay);
	void setU(double newU);
    void setNoiseDeviation(double val);

	void setYBorderTop(double newBorder);
	void setYBorderBottom(double newBorder);
	void setUBorderTop(double newBorder);
	void setUBorderBottom(double newBorder);

    void set_Coefficients(const std::vector<double>& A, const std::vector<double>& B) {
        ACoefficients = A;
        BCoefficients = B;
    }

    void setEnableU(bool enable) { areBordersU_Enabled = enable; }
    void setEnableY(bool enable) { areBordersY_Enabled = enable; }

    //Gettery
    double getYBorderTop() const { return y.top; }
    double getYBorderBottom() const { return y.bottom; }

    double getUBorderTop() const { return u.top; }
    double getUBorderBottom() const { return u.bottom; }

    bool get_EnableU() const { return areBordersU_Enabled; }
    bool get_EnableY() const { return areBordersY_Enabled; }

    //bool getBordersEnabled() const { return areBordersEnabled; }
    int get_K() const { return k; }
    double getNoiseDeviation() const { return noise_deviation; }
    double getZ() const { return z; }

    std::vector<double> getACoefficients() const {
        return ACoefficients;
    }
    std::vector<double> getBCoefficients() const {
        return BCoefficients;
    }

    double getLastU() const {
        if (!uHistory.empty())
            return uHistory.back();
        else
            return 0.0;
    }

    //Symulacja
	double simulate(double inputU);
    void reset();
};

