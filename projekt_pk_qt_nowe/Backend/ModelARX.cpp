#include "ModelARX.h"
#include <deque>
#include <iostream>

//Globalka
int G_ARX_MAX_HISTORY_LENGTH = 1000;

void ARX::setY(double newY) {
    if(this->areBordersY_Enabled){
        if (newY > this->y.top)
            newY = this->y.top;
        else if (newY < this->y.bottom)
            newY = this->y.bottom;
    }

    this->yHistory.push_back(newY);

    if (this->yHistory.size() > G_ARX_MAX_HISTORY_LENGTH){
        this->yHistory.pop_front();
    }
}

//KONSTRUKTOR

ARX::ARX(std::vector<double> A, std::vector<double> B, int k, double u) {
    this->BCoefficients = B;
    this->ACoefficients = A;
    if (k < 1) k = 1;
    this->k = k;
    this->uHistory.push_back(u);
    this->yHistory.push_back(0.0);

    this->z = 0.0; // Wartość początkowa zmiennej z

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator.seed(seed);
    setNoiseDeviation(0.0);
}

//SETTERY

void ARX::setACoefficient(int id, double value) {
    if (id >= this->ACoefficients.size()) {
        for (int i = this->ACoefficients.size(); i <= id; i++)
            this->ACoefficients.push_back(0.0);
    }
    this->ACoefficients[id] = value;
}

void ARX::setBCoefficient(int id, double value) {
    if (id >= this->BCoefficients.size()) {
        for (int i = this->BCoefficients.size(); i <= id; i++)
            this->BCoefficients.push_back(0.0);
    }
    this->BCoefficients[id] = value;
}

void ARX::setK(int newDelay) {
    if (newDelay < 1) newDelay = 1;
    this->k = newDelay;
}

void ARX::setU(double newU) {
    if(this->areBordersU_Enabled){
        if (newU > this->u.top)
            newU = this->u.top;
        else if (newU < this->u.bottom)
            newU = this->u.bottom;
    }

    this->uHistory.push_back(newU);

    if (this->uHistory.size() > G_ARX_MAX_HISTORY_LENGTH){
        this->uHistory.pop_front();
    }
}

void ARX::setNoiseDeviation(double newStdDev) {
    if (newStdDev < 0.0) newStdDev = 0.0; // Odchylenie musi być dodatnie
    this->noise_deviation = newStdDev;
    // Aktualizujemy rozkład: średnia 0.0, odchylenie stdDev
    this->distribution = std::normal_distribution<double>(0.0, this->noise_deviation);
}

void ARX::setYBorderTop(double newBorder) {
    if (newBorder < this->y.bottom)
        newBorder = this->y.bottom;
    this->y.top = newBorder;
}

void ARX::setYBorderBottom(double newBorder) {
    if (newBorder > this->y.top)
        newBorder = this->y.top;
    this->y.bottom = newBorder;
}

void ARX::setUBorderTop(double newBorder) {
    if (newBorder < this->u.bottom)
        newBorder = this->u.bottom;
    this->u.top = newBorder;
}

void ARX::setUBorderBottom(double newBorder) {
    if (newBorder > this->u.top)
        newBorder = this->u.top;
    this->u.bottom = newBorder;
}

//SYMULACJA

double ARX::simulate(double inputU) {
    double newY = 0.0;

    auto uHistoryForB = this->uHistory;
    auto yHistoryForA = this->yHistory;

    for (int i = 0; i < this->k - 1; i++) {
        if (uHistoryForB.size() <= 1) {
            break;
        }
        uHistoryForB.pop_back();
    }

    for (int i = 0; i < this->k - 2; i++) {

        if (yHistoryForA.size() <= 1) {
            break;
        }
        yHistoryForA.pop_back();
    }

    double ySumB = 0.0;
    double ySumA = 0.0;

    for (int i = 0; i < this->BCoefficients.size(); i++) {
        double tempU;
        double tempY;
        if (!uHistoryForB.empty())
            tempU = uHistoryForB.back();
        else
            tempU = 0.0;

        ySumB += this->BCoefficients[i] * tempU;
        if (!uHistoryForB.empty())
            uHistoryForB.pop_back();


        if (!yHistoryForA.empty())
            tempY = yHistoryForA.back();
        else
            tempY = 0.0;
        ySumA -= this->ACoefficients[i] * tempY;
        if (!yHistoryForA.empty())
            yHistoryForA.pop_back();
    }

    if (this->noise_deviation > 0.0) {
        this->z = distribution(generator);
    } else {
        this->z = 0.0;
    }

    newY = ySumB + ySumA + this->z;

    this->setY(newY);
    this->setU(inputU);
    //kluczowe, bo inaczej zwrocimy y przed nalozeniem ograniczen
    return yHistory.back();
}

void ARX::reset() {
    uHistory.clear();
    yHistory.clear();
}
