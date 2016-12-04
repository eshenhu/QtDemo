#ifndef QEXTCHECKBOX_H
#define QEXTCHECKBOX_H

#include <QVector>
#include <QCheckBox>
#include "ui/abstractspinboxattr.h"
#include <QtCharts/QChartGlobal>

#include "driver/modelpoctype.h"

using namespace ModelPOC;

class JsonPVConfig;
class JsonGUIElement;

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class QExtCheckBox : public AbstractSpinBoxAttr
{
public:
    explicit QExtCheckBox(const JsonPVConfig& config, const JsonGUIElement& element);
    ~QExtCheckBox();

    // BasedSpinBoxAttrInf interface
    void update(const QModbus2DataUnit *data, Phase phase);

    // factory
    static QExtCheckBox* makeExtCheckBox(const JsonPVConfig& config, const JsonGUIElement& element);
    static const QExtCheckBox* searchExtCheckBox(const JsonGUIElement& element);
    static const QVector<QExtCheckBox *>& qExtSpinBoxList();

private:
    //QChartView* m_assoChartView;
public:
    static QVector<QExtCheckBox*> m_qExtSpinBoxList;

    QChartView *assoChartView() const;
    void setAssoChartView(QChartView *assoChartView);
};
#endif // QEXTCHECKBOX_H
