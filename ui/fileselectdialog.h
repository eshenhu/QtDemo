#ifndef FILESELECTDIALOG_H
#define FILESELECTDIALOG_H

#include <QDialog>

namespace Ui {
class FileSelectDialog;
}

class FileSelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileSelectDialog(QWidget *parent = 0);
    ~FileSelectDialog();

private:
    Ui::FileSelectDialog *ui;
};

#endif // FILESELECTDIALOG_H
