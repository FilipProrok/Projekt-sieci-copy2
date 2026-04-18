#include "wykres.h"

Wykres::Wykres(QCustomPlot* plotWidget, QString title, QString yLabel)
{
    m_plot = plotWidget;
    m_timeWindow = G_DEFAULT_TIME_WINDOW;

    m_plot->plotLayout()->insertRow(0);
    m_plot->plotLayout()->addElement(0, 0, new QCPTextElement(m_plot, title, QFont("sans", 10, QFont::Bold)));
    m_plot->yAxis->setLabel(yLabel);
    m_plot->xAxis->setLabel("Czas [s]");
    m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    m_plot->legend->setVisible(true);
    m_plot->legend->setFont(QFont("Helvetica", 8));
}

void Wykres::add_series(QString name, QColor color, bool isStep)
{
    QCPGraph* graph = m_plot->addGraph();
    graph->setName(name);
    graph->setPen(QPen(color));

    if (!isStep)
        //graph->setLineStyle(QCPGraph::lsStepLeft);
        graph->setLineStyle(QCPGraph::lsStepCenter);
    else
        graph->setLineStyle(QCPGraph::lsLine);

    m_activeGraphs.append(graph);
}

void Wykres::setStepStyle(int seriesIndex, bool isStep)
{
    // Zabezpieczenia
    if (seriesIndex < 0 || seriesIndex >= m_activeGraphs.size()) return;

    QCPGraph* oldGraph = m_activeGraphs[seriesIndex];
    //QCPGraph::LineStyle newStyle = isStep ? QCPGraph::lsStepLeft : QCPGraph::lsLine;
    QCPGraph::LineStyle newStyle = isStep ? QCPGraph::lsLine : QCPGraph::lsStepCenter;

    if (oldGraph->lineStyle() == newStyle) return;

    // === TWORZENIE NOWEGO SEGMENTU === <- jedno z ciekawszych tutaj rozwiązań, bo to jest dosłownie łączenie 2 stylów wykresów (powód: QCPGraph nie pozwala na zmianę stylu w locie, przez co wykresy wyglądały gorzej)

    // Nowy wykres
    QCPGraph* newGraph = m_plot->addGraph();

    // Wartości z poprzedniego
    newGraph->setName(oldGraph->name());
    newGraph->setPen(oldGraph->pen());
    newGraph->setLineStyle(newStyle);

    // Łączenie poprzedniego i nowego wykresu
    if (!oldGraph->data()->isEmpty()) {
        // Pobieramy iterator do ostatniego elementu
        auto lastPointIt = oldGraph->data()->end();
        lastPointIt--; // Cofamy się o jeden, bo end() wskazuje "za" koniec

        double lastTime = lastPointIt->key;
        double lastValue = lastPointIt->value;

        newGraph->addData(lastTime, lastValue);
    }

    oldGraph->removeFromLegend();

    m_activeGraphs[seriesIndex] = newGraph;

    m_plot->replot();
}

void Wykres::add_point(int seriesIndex, double time, double value)
{
    if (seriesIndex < 0 || seriesIndex >= m_activeGraphs.size()) return;

    QCPGraph* currentGraph = m_activeGraphs[seriesIndex];

    currentGraph->addData(time, value);

    double cutoffTime = time - m_timeWindow - 2.0;

    // Iterujemy po wszystkich graphach jakie istnieją na plocie
    for (int i = 0; i < m_plot->graphCount(); ++i) {
        QCPGraph* g = m_plot->graph(i);
        // Usuwamy dane starsze niż okno czasowe
        g->data()->removeBefore(cutoffTime);
    }

    m_plot->xAxis->setRange(time, m_timeWindow, Qt::AlignRight);

    double globalMin = G_VERY_LARGE_NUMBER;
    double globalMax = G_VERY_SMALL_NUMBER;
    bool foundAnyData = false;

    // Szukamy min/max po WSZYSTKICH wykresach widocznych w oknie X
    for (int i = 0; i < m_plot->graphCount(); ++i) {
        bool foundRange;
        // getValueRange sprawdza tylko punkty widoczne w zakresie osi X
        QCPRange range = m_plot->graph(i)->getValueRange(foundRange, QCP::sdBoth, m_plot->xAxis->range());

        if (foundRange) {
            foundAnyData = true;
            if (range.lower < globalMin) globalMin = range.lower;
            if (range.upper > globalMax) globalMax = range.upper;
        }
    }

    if (foundAnyData) {
        double rangeSize = globalMax - globalMin;
        if (rangeSize < G_MIN_RANGE_SIZE ) {
            rangeSize = 1.0;
            if (qAbs(globalMax) < G_MIN_GLOBAL_MAX) {
                globalMin = -0.5;
                globalMax = 0.5;
            }
        }
        double margin = rangeSize * G_MARGIN_FACTOR;
        m_plot->yAxis->setRange(globalMin - margin, globalMax + margin);
    } else {
        m_plot->yAxis->setRange(-1, 1);
    }

    m_plot->replot();
}

void Wykres::clear()
{
    m_plot->clearGraphs(); // Usuwa wszystkie obiekty graph
    m_activeGraphs.clear(); // Czyści wskaźniki

    m_plot->replot();
}

void Wykres::setTimeWindow(double seconds)
{
    if (seconds >= G_DEFAULT_TIME_WINDOW) {
        m_timeWindow = seconds;
    }
}

void Wykres::reverse_legend_order(){ //metoda tylko dla wykresów z dwoma seriami, gdy chcemy by ta pierwsza w legendzie była rysowana na samej górze
    if(m_activeGraphs.size() < 2) return;

    QCPGraph* bottomGraph = m_activeGraphs.first();
    bottomGraph->removeFromLegend();
    bottomGraph->addToLegend();

    m_plot->replot();
}
