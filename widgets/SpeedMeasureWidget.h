#ifndef SPEEDMEASUREWIDGET_H
#define SPEEDMEASUREWIDGET_H

#include <QWidget>
#include <QLabel>

#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>

class SpeedMeasureWidget : public QWidget
{
    Q_OBJECT
public:
    static SpeedMeasureWidget *instance()
    {
        static SpeedMeasureWidget instance;
        return &instance;
    }
    ~SpeedMeasureWidget();
    void onCytometerSettingChanged();

    int preChannelId() const;
    int postChannelId() const;
    int distance() const;
    int preChannelThresh() const;


signals:
    void speedMeasureSettingChangged(int preCh, int postCh, int preThresh, int dist, int maxTimeSpan);

private:
    explicit SpeedMeasureWidget(QWidget *parent = nullptr);
    SpeedMeasureWidget &operator=(const SpeedMeasureWidget &) = delete;
    SpeedMeasureWidget(const SpeedMeasureWidget &) = delete;

    void initWidget();
    QComboBox *comboPreCh;
    QComboBox *comboPostCh;
    QSpinBox *spinDist;
    QSpinBox *spinMaxTimeDiff;
    QPushButton *btnConfirm;
    QLineEdit *editPreChThresh;

    int m_preChId;
    int m_postChId;
    int m_dist;
    int m_preChThresh;
    int m_maxTimeSpan;
};

inline int SpeedMeasureWidget::preChannelId() const
{
    return m_preChId;
}

inline int SpeedMeasureWidget::postChannelId() const
{
    return m_postChId;
}

inline int SpeedMeasureWidget::distance() const
{
    return m_dist;
}

inline int SpeedMeasureWidget::preChannelThresh() const
{
    return m_preChThresh;
}

#endif // SPEEDMEASUREWIDGET_H
