#include "actionwidget.h"
#include "mainwindow.h"
#include <QAction>
#include <QToolBar>
#include <qlabel.h>
#include <qstatusbar.h>
#include "util/qserialporthelper.h"

MainWindow::MainWindow(QWidget* parent)
{
    createActions();
    createStatusBar();
    m_widget = new ActionWidget();
    m_widget->setSettingDialog(settingDialog);
    setCentralWidget(m_widget);

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

void MainWindow::createActions()
{
    QToolBar *fileToolBar = addToolBar(tr("Config"));
//    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
//    QAction *newAct = new QAction(newIcon, tr("COM"), this);
//    newAct->setShortcuts(QKeySequence::New);
//    newAct->setStatusTip(tr("Config a COM port"));
//    connect(newAct, &QAction::triggered, this, &MainWindow::configCOM);
//    fileToolBar->addAction(newAct);

//    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
//    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
//    openAct->setShortcuts(QKeySequence::Open);
//    openAct->setStatusTip(tr("Open an existing file"));
//    connect(openAct, &QAction::triggered, this, &MainWindow::open);
//    fileToolBar->addAction(openAct);

//    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
//    QAction *saveAct = new QAction(saveIcon, tr("&Save"), this);
//    saveAct->setShortcuts(QKeySequence::Save);
//    saveAct->setStatusTip(tr("Save the document to disk"));
//    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
//    fileToolBar->addAction(saveAct);

//    connect(textEdit, &QPlainTextEdit::copyAvailable, cutAct, &QAction::setEnabled);
//    connect(textEdit, &QPlainTextEdit::copyAvailable, copyAct, &QAction::setEnabled);
}

void MainWindow::updateStatusBar(const QString& str)
{
    statusIndLabel->setText(str);
}
