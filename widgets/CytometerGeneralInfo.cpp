#include "CytometerGeneralInfo.h"
#include <QFormLayout>

CytometerGeneralInfo::CytometerGeneralInfo(QWidget *parent)
    : QWidget{parent}
{
    initWidget();
}

void CytometerGeneralInfo::onCytometerChanged(const QString &cytometerName, const QString &model, const QString &serial)
{
    lblCytometerName->setText(cytometerName);
    lblCytometerModel->setText(model);
    lblCytometerSerialNumber->setText(serial);
}

void CytometerGeneralInfo::onCytometerSettingsChanged(const CytometerSettings &cytometerSettings)
{
    lblCytometerSettingName->setText(cytometerSettings.name());
    lblSettingParent->setText(NodeTypeHelper::nodeTypeToString(cytometerSettings.parentType()) + QString::number(cytometerSettings.parentId()));
}


void CytometerGeneralInfo::initWidget()
{
    QFormLayout *layout = new QFormLayout(this);

    lblCytometerName = new QLabel("Seek Cytometer", this);
    lblCytometerModel = new QLabel("V1.0", this);
    lblCytometerSerialNumber = new QLabel("20251111", this);
    lblCytometerSettingName = new QLabel("Cytometer Settings", this);
    lblSettingParent = new QLabel("Experiment-", this);

    layout->addRow(tr("Cytometer Name:"), lblCytometerName);
    layout->addRow(tr("Cytometer Model:"), lblCytometerModel);
    layout->addRow(tr("Cytometer Serial Number:"), lblCytometerSerialNumber);
    layout->addRow(tr("Cytometer Setting Name:"), lblCytometerSettingName);
    layout->addRow(tr("Setting Parent:"), lblSettingParent);

    setLayout(layout);
}
