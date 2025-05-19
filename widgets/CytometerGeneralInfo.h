#ifndef CYTOMETERGENERALINFO_H
#define CYTOMETERGENERALINFO_H

#include <QWidget>
#include <QLabel>
#include "CytometerSettings.h"

class CytometerGeneralInfo : public QWidget
{
    Q_OBJECT
public:
    explicit CytometerGeneralInfo(QWidget *parent = nullptr);


public slots:
    void onCytometerChanged(const QString &cytometerName, const QString &model, const QString &serial);
    void onCytometerSettingsChanged(const CytometerSettings &cytometerSettings);

signals:

private:
    void initWidget();

    QLabel *lblCytometerName;
    QLabel *lblCytometerModel;
    QLabel *lblCytometerSerialNumber;
    QLabel *lblCytometerSettingName;
    QLabel *lblSettingParent;
};


#endif // CYTOMETERGENERALINFO_H
