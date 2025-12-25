#ifndef MICROFLUIDICWIDGET_H
#define MICROFLUIDICWIDGET_H

#include <QDockWidget>
#include <QObject>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>



class MicroFluidicWidget : public QDockWidget
{
    Q_OBJECT
public:
    MicroFluidicWidget(const QString &tilte, QWidget *parent = nullptr);


private:
    static constexpr int CHANNEL_NUM = 5;

    void initDockWidget();

    QPushButton *btnSoleValve[CHANNEL_NUM];
    QPushButton *btnPressControl[CHANNEL_NUM];
    QLabel      *lblPress[CHANNEL_NUM];
    QLabel      *lblPressControl[CHANNEL_NUM];
    QLabel      *lblSourePress;
    QSpinBox    *spinTargetPress[CHANNEL_NUM];
    QSpinBox    *spinPropoValveValue[CHANNEL_NUM];
};

#endif // MICROFLUIDICWIDGET_H
