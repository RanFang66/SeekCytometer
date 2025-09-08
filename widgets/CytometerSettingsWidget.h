#ifndef CYTOMETERSETTINGSWIDGET_H
#define CYTOMETERSETTINGSWIDGET_H

#include <QDockWidget>
#include <QObject>
#include "CytometerSettings.h"
#include "CytometerGeneralInfo.h"
#include "DetectorSettingsWidget.h"
#include "SpeedMeasureWidget.h"

class CytometerSettingsWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit CytometerSettingsWidget(const QString &title, QWidget *parent = nullptr);
    ~CytometerSettingsWidget();

    const QList<DetectorSettings> &detectorSettings() const;

public slots:
    void onCytometerSettingsChanged(int cytometerSettingId);

signals:

private:
    void initDockWidget();
    CytometerSettings       m_cytometerSettings;
    DetectorSettingsWidget  *m_detectorSettingsWidget;
    CytometerGeneralInfo    *m_generalInfoWidget;
    SpeedMeasureWidget      *m_speedMeasureWidget;
};

#endif // CYTOMETERSETTINGSWIDGET_H
