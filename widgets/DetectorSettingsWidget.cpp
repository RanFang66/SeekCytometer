#include "DetectorSettingsWidget.h"
#include <QTableView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>

#include <QDialog>


DetectorSettingsWidget::DetectorSettingsWidget(DetectorSettingsModel *model, QWidget *parent)
    : QWidget{parent}, m_dataModel{model}
{
    initWidget();
}

void DetectorSettingsWidget::initWidget()
{
    QTableView *tableView = new QTableView(this);
    tableView->setEditTriggers(QAbstractItemView::DoubleClicked);


    tableView->setModel(m_dataModel);

    QPushButton *btnAdd = new QPushButton("Add", this);
    QPushButton *btnRemove = new QPushButton("Remove", this);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QVBoxLayout *layout = new QVBoxLayout(this);
    btnLayout->addWidget(btnAdd);
    btnLayout->addWidget(btnRemove);
    layout->addWidget(tableView);
    layout->addLayout(btnLayout);
    setLayout(layout);

    connect(btnAdd, &QPushButton::clicked, this, [this](){
        m_dataModel->addDetectorSettings(DetectorSettings(0, 0, "FSC", 100, 0, true, 0, true, false, false));
    });
    connect(btnRemove, &QPushButton::clicked, this, [this](){
        m_dataModel->removeDetectorSettings(0);
    });
}

void DetectorSettingsWidget::onAddNewDetectorSetting()
{
    QDialog *dlg = new QDialog(this);
    QVBoxLayout *layout = new QVBoxLayout(dlg);
}
