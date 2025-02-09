#include "AddNewItemDialog.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>


AddNewItemDialog::AddNewItemDialog(const QString &newItemName, QWidget *parent)
    :QDialog{parent}, m_newItemName(newItemName)
{
    initDialog();
}

QString AddNewItemDialog::getItemName()
{
    return editName->text().trimmed();
}

void AddNewItemDialog::initDialog()
{
    promptStr = QString("%1 %2").arg(m_newItemName, tr("Name"));
    defaultName = QString("New%1").arg(m_newItemName);

    // switch (m_type) {
    // case NewItemType::Experiment:
    //     promptStr = tr("Experiment Name");
    //     defaultName = "NewExperiment";
    //     break;
    // case NewItemType::Specimen:
    //     promptStr = tr("Specimen Name");
    //     defaultName = "NewSpecimen";
    //     break;
    // case NewItemType::Tube:
    //     promptStr = tr("Tube Name");
    //     defaultName = "NewTube";
    //     break;
    // case NewItemType::Settings:
    //     promptStr = tr("Setting Name");
    //     defaultName = "NewSetting";
    //     break;
    // default:
    //     break;
    // }


    QLabel *lblName = new QLabel(promptStr, this);
    editName = new QLineEdit(defaultName, this);
    QFormLayout *layout = new QFormLayout();

    layout->addRow(lblName, editName);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *btnConfirm = new QPushButton(tr("Ok"), this);
    QPushButton *btnCancel = new QPushButton(tr("Cancel"), this);
    btnLayout->addWidget(btnConfirm);
    btnLayout->addWidget(btnCancel);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(layout);
    mainLayout->addLayout(btnLayout);
    setLayout(mainLayout);

    connect(btnConfirm, &QPushButton::clicked, this, &QDialog::accept);
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}


