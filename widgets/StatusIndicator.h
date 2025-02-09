#ifndef STATUSINDICATOR_H
#define STATUSINDICATOR_H

#include <QWidget>
#include <QPainter>

class StatusIndicator : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int status READ status WRITE setStatus NOTIFY statusChanged);

public:
    enum STATUS {
        STATUS_IDLE =0,
        STATUS_RUNNING,
        STATUS_FAULT,
    };

    explicit StatusIndicator(QWidget *parent = nullptr);

    int status() const;
    QSize sizeHint() const override;
    void setStatus(int newStatus);

signals:
    void statusChanged(int newStatus);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int m_status;
};

inline int StatusIndicator::status() const
{
    return m_status;
}

inline QSize StatusIndicator::sizeHint() const
{
    return QSize(30, 30);
}

#endif // STATUSINDICATOR_H
