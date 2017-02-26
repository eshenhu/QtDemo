#include "fileselectdialog.h"
#include "ui_fileselectdialog.h"

#include <QTimer>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

FileSelectDialog::FileSelectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileSelectDialog),
    cfgElementList(MAX_ROW_FILE_SELECTION),
    m_cntFileSelection(1)
{
    ui->setupUi(this);

    m_rowFileSelection.push_back(CompRowFileSelectionClz(ui->lineEdit_filePath, ui->toolButton_open));
    m_rowFileSelection.push_back(CompRowFileSelectionClz(ui->lineEdit_filePath_2, ui->toolButton_open_2));
    m_rowFileSelection.push_back(CompRowFileSelectionClz(ui->lineEdit_filePath_3, ui->toolButton_open_3));
    m_rowFileSelection.push_back(CompRowFileSelectionClz(ui->lineEdit_filePath_4, ui->toolButton_open_4));
    m_rowFileSelection.push_back(CompRowFileSelectionClz(ui->lineEdit_filePath_5, ui->toolButton_open_5));
    m_rowFileSelection.push_back(CompRowFileSelectionClz(ui->lineEdit_filePath_6, ui->toolButton_open_6));
    m_rowFileSelection.push_back(CompRowFileSelectionClz(ui->lineEdit_filePath_7, ui->toolButton_open_7));
    m_rowFileSelection.push_back(CompRowFileSelectionClz(ui->lineEdit_filePath_8, ui->toolButton_open_8));

    initSetupUi();
    setupSignalAndSlot();
}

FileSelectDialog::~FileSelectDialog()
{
    delete ui;
}

void FileSelectDialog::initSetupUi()
{
    m_rowFileSelection[0].lineEdit->setText("");
    for (int i = MIN_ROW_FILE_SELECTION; i<MAX_ROW_FILE_SELECTION; i++)
    {
        m_rowFileSelection[i].lineEdit->setText("");
        m_rowFileSelection[i].lineEdit->hide();
        m_rowFileSelection[i].toolBtn->hide();
    }
}

void FileSelectDialog::setupSignalAndSlot()
{
    QTimer::singleShot(10, [this](){
        QSize size = this->sizeHint();
        this->resize(size);
    });
    connect(this->ui->pushButton_plus, &QPushButton::clicked, this, &FileSelectDialog::createFileCmpBox);
    connect(this->ui->pushButton_minus, &QPushButton::clicked, this, &FileSelectDialog::deleteFileCmpBox);
    connect(this->ui->buttonBox, &QDialogButtonBox::accepted, this, &FileSelectDialog::validate);
    connect(this->ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

   //connect(ui->toolButton_open, SIGNAL(clicked(bool)), this, SLOT(defaultAction(bool)));
//    connect(ui->toolButton_open_2, SIGNAL(clicked(bool)), this, SLOT(defaultAction(bool)));
//    connect(ui->toolButton_open_3, SIGNAL(clicked(bool)), this, SLOT(defaultAction(bool)));
    for(const CompRowFileSelectionClz& item : m_rowFileSelection)
    {
        connect(item.toolBtn, SIGNAL(clicked(bool)), this, SLOT(defaultAction(bool)));
    }

}

void FileSelectDialog::createFileCmpBox()
{
    if (!cntUpFileSelection())
    {
        QMessageBox::warning(this, tr("Warning"), tr("<p>Only support comparing data with maximum 8 files"),
                             QMessageBox::Ok);
    }

    for (quint32 i=0; i<m_cntFileSelection; ++i )
    {
        m_rowFileSelection[i].lineEdit->show();
        m_rowFileSelection[i].toolBtn->show();
    }

    for (quint32 i=m_cntFileSelection; i<MAX_ROW_FILE_SELECTION; ++i )
    {
        m_rowFileSelection[i].lineEdit->hide();
        m_rowFileSelection[i].toolBtn->hide();
    }

    ui->gridLayout->invalidate();

    QTimer::singleShot(10, [this](){
        QSize size = this->sizeHint();
        this->resize(size);
    });
}

/*
 * we always delete the last one;
*/
void FileSelectDialog::deleteFileCmpBox()
{
    if (!cntDwnFileSelection())
        return;

    for (quint32 i=0; i<m_cntFileSelection; ++i )
    {
        m_rowFileSelection[i].lineEdit->show();
        m_rowFileSelection[i].toolBtn->show();
    }

    for (quint32 i=m_cntFileSelection; i<MAX_ROW_FILE_SELECTION; ++i )
    {
        m_rowFileSelection[i].lineEdit->setText("");
        m_rowFileSelection[i].lineEdit->hide();
        m_rowFileSelection[i].toolBtn->hide();
    }

    ui->gridLayout->invalidate();

    QTimer::singleShot(10, [this](){
        QSize size = this->sizeHint();
        this->resize(size);
    });
}

void FileSelectDialog::defaultAction(bool)
{
    QToolButton* send = qobject_cast<QToolButton*>(sender());
    QString path = QCoreApplication::applicationDirPath();
    const QString fileName = QFileDialog::getOpenFileName(this,
                                                          tr("Open Json File"),
                                                          path,
                                                          tr("Json Files (*.json)"));

    if (fileName.isNull())
        return;

    for(const CompRowFileSelectionClz& item : m_rowFileSelection)
    {
        if (item.toolBtn == send)
        {
            item.lineEdit->setReadOnly(true);
            item.lineEdit->setText(fileName);
            break;
        }
    }
}

void FileSelectDialog::validate()
{
    CfgResHandlerInf::ProductVersion pv_0;
    TestPlanEnum plan_0;

    for (ChartViewCfgElement& ele : cfgElementList)
    {
        ele.reset();
    }

    for (quint32 i = 0; i < m_cntFileSelection; i++)
    {
        if (m_rowFileSelection[i].lineEdit->text().isEmpty())
        {
            QMessageBox::warning(this, tr("Warning"),
                                 tr("<p>Empty file name was not allowed!"),
                                 QMessageBox::Ok);
            goto goto_end;
        }
    }

    for (quint32 i = 0; i < m_cntFileSelection; i++)
    {
        const QString filename = m_rowFileSelection[i].lineEdit->text();
        if (!openJsonFile(filename, i))
        {
            goto goto_end;
        }
    }

    pv_0 = cfgElementList[0].cfgMetaData.pv();
    plan_0 = cfgElementList[0].cfgMetaData.plan();

    for (quint32 i = 0; i < m_cntFileSelection; i++)
    {
        CfgResHandlerInf::ProductVersion pv = cfgElementList[i].cfgMetaData.pv();
        TestPlanEnum plan = cfgElementList[i].cfgMetaData.plan();

        if ( pv != pv_0 || plan != plan_0)
        {
            QMessageBox warningBox(QMessageBox::Warning, tr("Warning"),
                                   tr("<p>Those files was not compitable either"
                                      "<p>- Test Plan   "
                                      "<p>- Product Version "),
                                   QMessageBox::Close);
            warningBox.exec();

            goto goto_end;
        }
    }


goto_end:
    this->accept();
}

//void ChartViewerWin::open()
//{
//    QString path = QCoreApplication::applicationDirPath();
//    const QString fileName = QFileDialog::getOpenFileName(this,
//                                                          tr("Open Json File"),
//                                                          path,
//                                                          tr("Json Files (*.json)"));
//    if (!fileName.isEmpty())
//    {
//        for (ChartViewerWin::ChartViewCfgElement& ele : cfgElementList)
//        {
//            ele.reset();
//        }

//        if (openJsonFile(fileName, (quint32)ChartViewerWin::ChartViewIdxSupFileE::IDX_SUP_FILE_I))
//        {
//            releaseSignalAndSlot();
//            initAxesAndView(ui->customPlot);
//            loadDefault2Plot();
//            setupSignalAndSlot();

//            emit testPlanChanged(cfgMetaData.plan());
//            // inform this information
//            //emit leftFileOk(true);
//        }
//        ui->customPlot->replot();
//    }
//}

/* 1 : OK
*  0 : Fail
*/

#define RETURN_WITH_OK   {isOk = true;  break;}
#define RETURN_WITH_NOK  {isOk = false; break;}

#include "cfg/datajsoncfgreader.h"

bool FileSelectDialog::openJsonFile(const QString& jsonFileName, quint32 location)
{
    bool isOk = false;
    cfgElementList[location].reset();

    while (location < MAX_ROW_FILE_SELECTION)
    {
        DataJsonCfgReader reader;

        if (!reader.loadData(jsonFileName))
        {
            QMessageBox warningBox(QMessageBox::Warning, tr("Warning"),
                                   tr("This file can't be read with reason of either \n"
                                      " - json file format was corrupted. \n"
                                      " - data file was corrupted. \n"
                                      " - checksum was not right"),
                                   QMessageBox::Close);
            warningBox.exec();
            RETURN_WITH_NOK
        }
        cfgElementList[location].cfgCsvFullPathName = reader.getFullPathCSVName();
        cfgElementList[location].cfgMetaData = reader.getCfgParser();
        cfgElementList[location].cfgRawData = reader.csvDataHandler();

        RETURN_WITH_OK
    }
    return isOk;
}
