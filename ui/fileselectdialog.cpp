#include "fileselectdialog.h"
#include "ui_fileselectdialog.h"

#include <QTimer>
#include <QDebug>
#include <QMessageBox>

FileSelectDialog::FileSelectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileSelectDialog),
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
    for (int i = MIN_ROW_FILE_SELECTION; i<MAX_ROW_FILE_SELECTION; i++)
    {
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
//    ui->lineEdit_filePath_2->show();
//    ui->toolButton_open_2->show();
//    ui->lineEdit_filePath_3->show();
//    ui->toolButton_open_3->show();
//    ui->lineEdit_filePath_4->show();
//    ui->toolButton_open_4->show();
//    ui->lineEdit_filePath_5->show();
//    ui->toolButton_open_5->show();
//    ui->lineEdit_filePath_6->show();
//    ui->toolButton_open_6->show();
//    ui->lineEdit_filePath_7->show();
//    ui->toolButton_open_7->show();
//    ui->lineEdit_filePath_8->show();
//    ui->toolButton_open_8->show();
;
//    int index = ui->verticalLayout_2->count() - 1;
//    QLayoutItem* item = ui->verticalLayout_2->takeAt(index);
//    QLayout* layout = item->layout();
//    ui->verticalLayout_2->removeItem(item);

//    delete item;
//    for (int i = 0; i < layout->count(); ++i)
//    {
//        delete layout->itemAt(i);
//    }

    if (!cntDwnFileSelection())
        return;

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
