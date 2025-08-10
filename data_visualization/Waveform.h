#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <QChart>
#include <QObject>

QT_FORWARD_DECLARE_CLASS(QGestureEvent)

class Waveform : public QChart
{
    Q_OBJECT
public:
    explicit Waveform(QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = {});

protected:
    bool sceneEvent(QEvent *event) override;

private:
    bool gestureEvent(QGestureEvent *event);
};

#endif // WAVEFORM_H
