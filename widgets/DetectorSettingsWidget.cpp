#include "DetectorSettingsWidget.h"
#include <QTableView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include "AddNewDetectorDialog.h"
#include "CheckBoxDelegate.h"

DetectorSettingsWidget::DetectorSettingsWidget(DetectorSettingsModel *model, QWidget *parent)
    : QWidget{parent}, m_dataModel{model}
{
    initWidget();
}

void DetectorSettingsWidget::initWidget()
{
    QTableView *tableView = new QTableView(this);
    tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);


    tableView->setModel(m_dataModel);

    CheckBoxDelegate *delegate = new CheckBoxDelegate(tableView);
    tableView->setItemDelegateForColumn(DetectorSettingsModel::EnableHeightColumn, delegate);
    tableView->setItemDelegateForColumn(DetectorSettingsModel::EnableAreaColumn, delegate);
    tableView->setItemDelegateForColumn(DetectorSettingsModel::EnableWidthColumn, delegate);
    tableView->setItemDelegateForColumn(DetectorSettingsModel::EnableThresholdColumn, delegate);

    QPushButton *btnAdd = new QPushButton("Add", this);
    QPushButton *btnRemove = new QPushButton("Remove", this);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QVBoxLayout *layout = new QVBoxLayout(this);
    btnLayout->addWidget(btnAdd);
    btnLayout->addWidget(btnRemove);
    layout->addWidget(tableView);
    layout->addLayout(btnLayout);
    setLayout(layout);

    connect(btnAdd, &QPushButton::clicked, this, &DetectorSettingsWidget::onAddNewDetectorSetting);
    connect(btnRemove, &QPushButton::clicked, this, [this](){
        m_dataModel->removeDetectorSettings(0);
    });
}

void DetectorSettingsWidget::onAddNewDetectorSetting()
{
    AddNewDetectorDialog *dlg = new AddNewDetectorDialog(this);
    int ret = dlg->exec();
    if (ret == QDialog::Accepted) {
        QList<Detector> seletectedDetectors = dlg->getSelectedDetectors();
        for (const Detector &detector : seletectedDetectors) {
            m_dataModel->addDetectorSettings(DetectorSettings(0, detector.id(), detector.name()));
        }
    }

    delete dlg;
}
