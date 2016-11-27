#include "qextcheckbox.h"
#include "ui/qrtlineseries.h"
#include <QChartView>

QVector<QExtCheckBox*> QExtCheckBox::m_qExtSpinBoxList = QVector<QExtCheckBox*>();

QT_CHARTS_USE_NAMESPACE

QExtCheckBox::QExtCheckBox(QWidget *parent): m_assoChartView(nullptr)
{
}

QExtCheckBox::QExtCheckBox(const JsonPVConfig &config, const JsonGUIElement &element, QWidget *parent):
    QCheckBox(element.str(), parent),
    AbstractSpinBoxAttr(config, element),
    m_assoChartView(nullptr)
{
}

QExtCheckBox::~QExtCheckBox()
{
}

void QExtCheckBox::update(const QModbus2DataUnit *data)
{
    AbstractSpinBoxAttr::update(data);
    if (QCheckBox::isChecked() && m_assoChartView != nullptr)
    {
        QChart* chart = m_assoChartView->chart();

        QStringList title = chart->title().split(':');
        QStringList newTitle;
        newTitle << title[0] << QString::number(this->pushData());
        chart->setTitle(newTitle.join(':'));

        foreach (QAbstractSeries* series, chart->series())
        {
            static_cast<QRTLineSeries*>(series)->update(this->pushData());
        }
    }
}

QExtCheckBox *QExtCheckBox::makeExtCheckBox(const JsonPVConfig &config, const JsonGUIElement &element, QWidget *parent)
{
    QExtCheckBox::m_qExtSpinBoxList.append(new QExtCheckBox(config, element, parent));
    return m_qExtSpinBoxList.last();
}

const QVector<QExtCheckBox *>& QExtCheckBox::qExtSpinBoxList()
{
    return m_qExtSpinBoxList;
}

QChartView *QExtCheckBox::assoChartView() const
{
    return m_assoChartView;
}

void QExtCheckBox::setAssoChartView(QChartView *assoChartView)
{
    m_assoChartView = assoChartView;
}
