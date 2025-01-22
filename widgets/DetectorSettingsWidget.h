#ifndef DETECTORSETTINGSWIDGET_H
#define DETECTORSETTINGSWIDGET_H

#include <QWidget>
#include "DetectorSettingsModel.h"

class DetectorSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DetectorSettingsWidget(DetectorSettingsModel *model, QWidget *parent = nullptr);

signals:

private:
    void initWidget();
    DetectorSettingsModel *m_dataModel;


private slots:
    void onAddNewDetectorSetting();
};

#endif // DETECTORSETTINGSWIDGET_H
