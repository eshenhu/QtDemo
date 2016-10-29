#include "qrtlineseries.h"


QRTLineSeries::QRTLineSeries(QObject *parent):
    QLineSeries(parent)
{
}

QRTLineSeries::~QRTLineSeries()
{

}

void QRTLineSeries::update(double v)
{
    QVector<QPointF> oldPoints = this->pointsVector();
    QVector<QPointF> points;

    quint32 pCnt = oldPoints.count();
    if (pCnt < MAX_NUM_POINTS) {
        points = this->pointsVector();
    } else {
        int startPos = MAX_NUM_POINTS - pCnt + 1;
        for (int i = startPos; i < oldPoints.count(); i++)
            points.append(QPointF(i, oldPoints.at(i).y()));
    }

    qint64 size = points.count();
    points.append(QPointF(size, v));

    this->replace(points);
}
