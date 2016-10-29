#ifndef QEXTCHECKBOX_H
#define QEXTCHECKBOX_H

#include <QVector>
#include <QCheckBox>
#include "ui/abstractspinboxattr.h"

class JsonPVConfig;
class JsonGUIElement;


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

public:
    static QVector<QExtCheckBox*> m_qExtSpinBoxList;
};
#endif // QEXTCHECKBOX_H
