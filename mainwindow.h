#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qlabel.h>

class ActionWidget;
class QSerialPortSetting;

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    void createStatusBar();
    void createActions();

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
private:
    ActionWidget* m_widget;
    QLabel *statusIndLabel;
signals:

public slots:
    void updateStatusBar(const QString&);
};

#endif // MAINWINDOW_H
