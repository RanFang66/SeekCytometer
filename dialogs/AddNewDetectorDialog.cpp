#include "AddNewDetectorDialog.h"
#include "DetectorModel.h"
#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

AddNewDetectorDialog::AddNewDetectorDialog(int settingId, QWidget *parent)
    : QDialog{parent}, m_settingId{settingId}
{
    initDialog();
}

QList<Detector> AddNewDetectorDialog::getSelectedDetectors()
{
    QList<Detector> detectors;
    if (m_selection->hasSelection()) {
        QModelIndexList selectedRows = m_selection->selectedRows();
        for (const QModelIndex &index : selectedRows) {
            detectors.append(m_model->getDetector(index.row()));
        }
    }
    qDebug() << QString("Selected %1 rows").arg(detectors.size());
    return detectors;
}

void AddNewDetectorDialog::initDialog()
{
    QTableView *detectorView = new QTableView(this);
    m_model = new DetectorModel(this);
    m_model->initDetectorModel(m_settingId);
    detectorView->setModel(m_model);
    m_selection = new QItemSelectionModel(m_model);
    detectorView->setSelectionModel(m_selection);
    detectorView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    detectorView->setSelectionBehavior(QAbstractItemView::SelectRows);




    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *btnOk = new QPushButton(tr("Ok"), this);
    QPushButton *btnCancel = new QPushButton(tr("Cancel"), this);

    btnLayout->addWidget(btnOk);
    btnLayout->addWidget(btnCancel);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(detectorView);
    layout->addLayout(btnLayout);
    setLayout(layout);


    connect(btnOk, &QPushButton::clicked, this, [this](){
        accept();
    });
    connect(btnCancel, &QPushButton::clicked, this, [this](){
        reject();
    });
}
