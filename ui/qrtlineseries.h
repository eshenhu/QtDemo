#ifndef QRTLINESERIES_H
#define QRTLINESERIES_H


#include <QObject>
#include <QLineSeries>

class QRTLineSeries : public QtCharts::QLineSeries
{
    Q_OBJECT

public:
    QRTLineSeries(QObject *parent = Q_NULLPTR);
    ~QRTLineSeries();

public:
    const static quint32 MAX_NUM_POINTS = 128;

public:
    void update(double v);
};

#endif // QRTLINESERIES_H
