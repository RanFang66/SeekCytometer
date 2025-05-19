#ifndef DETECTORSETTINGSWIDGET_H
#define DETECTORSETTINGSWIDGET_H

#include <QWidget>
#include <QTableView>

#include "DetectorSettingsModel.h"

class DetectorSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DetectorSettingsWidget(QWidget *parent = nullptr);
    void onCytometerSettingChanged(int settingId);
    const QList<DetectorSettings> &detectorSettings() const;


signals:

private:
    void initWidget();
    int  m_settingId;
    QTableView *tableView;
    DetectorSettingsModel *m_model;

private slots:
    void onAddNewDetectorSetting();
    void onDeleteDetectorSetting();

};

#endif // DETECTORSETTINGSWIDGET_H
