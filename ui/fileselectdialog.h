#ifndef FILESELECTDIALOG_H
#define FILESELECTDIALOG_H

#include <QDialog>
#include <QVector>
#include <QLineEdit>
#include <QToolButton>
#include <cfg/cfgjsonrecelement.h>
#include <cfg/cfgwashingdatainf.h>

namespace Ui {
class FileSelectDialog;
}

class ChartViewCfgElement
{
public:
    ChartViewCfgElement()
    {
        reset();
    }

    void reset(){
        cfgMetaData = CfgJsonRecElement();
        cfgRawData.reset();
    }

public:
    CfgJsonRecElement cfgMetaData;
    QSharedPointer<CfgWashingDataInf> cfgRawData;
};


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

public:
    QVector<ChartViewCfgElement> cfgElementList;

private:
    void setupSignalAndSlot();
    void initSetupUi();
    bool openJsonFile(const QString& jsonFileName, quint32 location);

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
    QVector<CompRowFileSelectionClz> m_rowFileSelection;
private slots:
    void createFileCmpBox();
    void deleteFileCmpBox();

    void defaultAction(bool);
    void validate();
};

#endif // FILESELECTDIALOG_H
