#ifndef FILESELECTDIALOG_H
#define FILESELECTDIALOG_H

#include <QDialog>
#include <QVector>
#include <QLineEdit>
#include <QToolButton>

namespace Ui {
class FileSelectDialog;
}

class FileSelectDialog : public QDialog
{
    Q_OBJECT
public:
    const static int MIN_ROW_FILE_SELECTION = 1;
    const static int MAX_ROW_FILE_SELECTION = 8;
    struct CompRowFileSelectionClz{
        CompRowFileSelectionClz()
        {
            lineEdit = nullptr;
            toolBtn = nullptr;
        }

        CompRowFileSelectionClz(QLineEdit* left, QToolButton* right)
        {
            lineEdit = left;
            toolBtn = right;
        }

        QLineEdit* lineEdit;
        QToolButton* toolBtn;
    };
public:
    explicit FileSelectDialog(QWidget *parent = 0);
    ~FileSelectDialog();

private:
    void setupSignalAndSlot();
    void initSetupUi();

private:
    bool cntUpFileSelection()
    {
        bool result = false;
        if (m_cntFileSelection < MAX_ROW_FILE_SELECTION)
        {
            ++m_cntFileSelection;
            result = true;
        }
        return result;
    }

    bool cntDwnFileSelection()
    {
        bool result = false;
        if (m_cntFileSelection > MIN_ROW_FILE_SELECTION)
        {
            --m_cntFileSelection;
            result = true;
        }
        return result;
    }

private:
    Ui::FileSelectDialog *ui;
    quint32 m_cntFileSelection;
private:
    QVector<CompRowFileSelectionClz> m_rowFileSelection;
private slots:
    void createFileCmpBox();
    void deleteFileCmpBox();
};

#endif // FILESELECTDIALOG_H
