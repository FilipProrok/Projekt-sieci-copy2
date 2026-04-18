#ifndef WYKRES_H
#define WYKRES_H

#include "qcustomplot.h"
#include <QVector>

class Wykres
{
public:
    Wykres(QCustomPlot* plotWidget, QString title, QString yLabel);

    void add_series(QString name, QColor color, bool isStep = false);
    void add_point(int seriesIndex, double time, double value);
    void setStepStyle(int seriesIndex, bool isStep);

    void clear();
    void setTimeWindow(double seconds);
    void reverse_legend_order();

private:
    QCustomPlot* m_plot;
    double m_timeWindow;

    QVector<QCPGraph*> m_activeGraphs;

    // Stale (aby uniknac magicznych liczb)
    double G_DEFAULT_TIME_WINDOW = 10.0;
    double G_VERY_LARGE_NUMBER = 1e9;
    double G_VERY_SMALL_NUMBER = -1e9;
    double G_MARGIN_FACTOR = 0.1; // 10% margines
    double G_MIN_RANGE_SIZE = 0.001;
    double G_MIN_GLOBAL_MAX = 0.0001;
};

#endif // WYKRES_H
