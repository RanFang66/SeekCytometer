#ifndef ADDNEWITEMDIALOG_H
#define ADDNEWITEMDIALOG_H

#include <QDialog>
#include <QLineEdit>


// enum class NewItemType {
//     Experiment,
//     Specimen,
//     Tube,
//     Settings,
// };

class AddNewItemDialog : public QDialog
{
    Q_OBJECT
public:
    AddNewItemDialog(const QString &newItemName, QWidget *parent = nullptr);
    QString getItemName();

private:
    void initDialog();
    // NewItemType m_type;
    QString m_newItemName;
    QString promptStr;
    QString defaultName;
    QLineEdit *editName;
};

#endif // ADDNEWITEMDIALOG_H
