#include "actionwidget.h"
#include "mainwindow.h"
#include <qlabel.h>
#include <qstatusbar.h>

MainWindow::MainWindow(QWidget* parent)
{
    m_widget = new ActionWidget();
    setCentralWidget(m_widget);

    createStatusBar();
    resize(900, 600);
}

MainWindow::~MainWindow()
{
    delete m_widget;
}

void MainWindow::createStatusBar()
{
    statusIndLabel = new QLabel;
    statusIndLabel->setIndent(7);

    statusBar()->addWidget(statusIndLabel);

    //connect(m_widget, SIGNAL(modified()), this, SLOT(spreadsheetModified()));
    updateStatusBar(tr("init"));
}

void MainWindow::updateStatusBar(const QString& str)
{
    statusIndLabel->setText(str);
}
