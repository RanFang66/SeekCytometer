#ifndef CYTOMETERSETTINGSWIDGET_H
#define CYTOMETERSETTINGSWIDGET_H

#include <QDockWidget>
#include <QObject>
#include "CytometerSettings.h"
#include "DetectorSettingsWidget.h"

class CytometerSettingsWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit CytometerSettingsWidget(const QString &title, QWidget *parent = nullptr);
    ~CytometerSettingsWidget();

signals:

private:
    void initDockWidget();
    DetectorSettingsModel   *m_detectorSettingsModel;
    DetectorSettingsWidget  *m_detectorSettingsWidget;
};

#endif // CYTOMETERSETTINGSWIDGET_H
