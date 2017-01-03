#include "fileselectdialog.h"
#include "ui_fileselectdialog.h"

FileSelectDialog::FileSelectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileSelectDialog)
{
    ui->setupUi(this);
}

FileSelectDialog::~FileSelectDialog()
{
    delete ui;
}
