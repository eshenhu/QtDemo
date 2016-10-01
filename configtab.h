#ifndef CONFIGTAB_H
#define CONFIGTAB_H

#include <QWidget>

class ConfigTab : public QWidget
{
    Q_OBJECT
    void createSubPanel();
public:
    explicit ConfigTab(QWidget *parent = 0);

signals:

public slots:
};

#endif // CONFIGTAB_H
