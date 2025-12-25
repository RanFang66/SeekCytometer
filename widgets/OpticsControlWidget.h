#ifndef OPTICSCONTROLWIDGET_H
#define OPTICSCONTROLWIDGET_H

#include <QDockWidget>
#include <QObject>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>


class OpticsControlWidget : public QDockWidget
{
    Q_OBJECT
public:
    OpticsControlWidget(const QString &tilte, QWidget *parent = nullptr);

private:
    void initDockWidget();

    QPushButton *btnLaser_1;
    QPushButton *btnLaser_2;
    QPushButton *btnLed;

    QSpinBox *spinLaserIntensity_1;
    QSpinBox *spinLaserIntensity_2;
    QSpinBox *spinLedIntensity;

    QLabel  *lblLaser1Status;
    QLabel  *lblLaser2Status;
    QLabel  *lblLedStatus;
};

#endif // OPTICSCONTROLWIDGET_H
