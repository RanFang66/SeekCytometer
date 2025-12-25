#ifndef SAMPLECHIPWIDGET_H
#define SAMPLECHIPWIDGET_H

#include <QDockWidget>
#include <QObject>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
class SampleChipWidget : public QDockWidget
{
    Q_OBJECT
public:
    SampleChipWidget(const QString &tilte, QWidget *parent = nullptr);

private:
    void initSampleChipWidget();


    QPushButton *btnMoveXForward;
    QPushButton *btnMoveXBackward;
    QPushButton *btnMoveYForward;
    QPushButton *btnMoveYBackward;
    QPushButton *btnMoveLenUp;
    QPushButton *btnMoveLenDown;


    QPushButton *btnOpenCover;
    QPushButton *btnCloseCover;
    QPushButton *btnPressSample;
    QPushButton *btnReleaseSample;

    QPushButton *btnChurnRunCW;
    QPushButton *btnChurnRunCCW;

    QPushButton *btnTempControl;

    QLabel *lblCoverStatus;
    QLabel *lblPressStatus;
    QLabel *lblChurnStatus;
    QLabel *lblTempContorlStatus;
    QLabel *lblChipPos;
    QLabel *lblLenPos;


    QSpinBox *spinChurnSpeed;
    QSpinBox *spinTargetTemp;

};


#endif // SAMPLECHIPWIDGET_H
