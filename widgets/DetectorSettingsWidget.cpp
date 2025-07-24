#include "DetectorSettingsWidget.h"
#include <QTableView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include "AddNewDetectorDialog.h"
#include "CheckBoxDelegate.h"
#include <QMessageBox>
#include "DetectorSettingsModel.h"

DetectorSettingsWidget::DetectorSettingsWidget(QWidget *parent)
    : QWidget{parent}, m_settingId(0), tableView(new QTableView(this)), m_model(DetectorSettingsModel::instance())
{
    initWidget();
}

void DetectorSettingsWidget::initWidget()
{
    tableView->setModel(m_model);
    tableView->setEditTriggers(QAbstractItemView::DoubleClicked);

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
    connect(btnRemove, &QPushButton::clicked, this, &DetectorSettingsWidget::onDeleteDetectorSetting);
}

void DetectorSettingsWidget::onCytometerSettingChanged(int settingId)
{
    m_settingId = settingId;
    m_model->resetDetectorSettingModel(m_settingId);
}

const QList<DetectorSettings> &DetectorSettingsWidget::detectorSettings() const
{
    return m_model->detectorSettings();
}

void DetectorSettingsWidget::onAddNewDetectorSetting()
{
    if (m_settingId == 0) {
        QMessageBox::warning(this, tr("No Experiment Selected"), tr("You should select an experiment first!"));
        return;
    }


    AddNewDetectorDialog *dlg = new AddNewDetectorDialog(m_settingId, this);
    int ret = dlg->exec();
    if (ret == QDialog::Accepted) {
        QList<Detector> seletectedDetectors = dlg->getSelectedDetectors();
        for (const Detector &detector : seletectedDetectors) {
            m_model->addDetectorSettings(DetectorSettings(m_settingId, detector));
        }
    }

    delete dlg;
}

void DetectorSettingsWidget::onDeleteDetectorSetting()
{
    QMessageBox::StandardButton ret = QMessageBox::question(this, tr("Delete Detector Setting"), tr("Confirm to delete the selected detector setting?"),
                          QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (ret == QMessageBox::No) {
        return;
    }
    m_model->removeDetectorSettings(tableView->currentIndex().row());
}
