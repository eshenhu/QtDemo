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
    static const QExtCheckBox* searchExtCheckBox(const JsonGUIPrimType type, const quint32 idxMotor);
    static const QVector<QExtCheckBox *>& qExtSpinBoxList();

private:
    //QChartView* m_assoChartView;
public:
    static QVector<QExtCheckBox*> m_qExtSpinBoxList;

};
#endif // QEXTCHECKBOX_H
