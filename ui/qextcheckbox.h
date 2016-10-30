#ifndef QEXTCHECKBOX_H
#define QEXTCHECKBOX_H

#include <QVector>
#include <QCheckBox>
#include "ui/abstractspinboxattr.h"
#include <QtCharts/QChartGlobal>

class JsonPVConfig;
class JsonGUIElement;

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class QExtCheckBox : public QCheckBox, public AbstractSpinBoxAttr
{
    Q_OBJECT

public:
    explicit QExtCheckBox(QWidget *parent = Q_NULLPTR);
    explicit QExtCheckBox(const JsonPVConfig& config, const JsonGUIElement& element, QWidget *parent = Q_NULLPTR);
    ~QExtCheckBox();

    // BasedSpinBoxAttrInf interface
    void update(const QModbus2DataUnit *data);

    // factory
    static QExtCheckBox* makeExtCheckBox(const JsonPVConfig& config, const JsonGUIElement& element, QWidget *parent = Q_NULLPTR);
    static const QVector<QExtCheckBox *>& qExtSpinBoxList();

signals:

public slots:

private:
    QChartView* m_assoChartView;
public:
    static QVector<QExtCheckBox*> m_qExtSpinBoxList;

    QChartView *assoChartView() const;
    void setAssoChartView(QChartView *assoChartView);
};
#endif // QEXTCHECKBOX_H
