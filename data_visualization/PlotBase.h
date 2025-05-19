#ifndef PLOTBASE_H
#define PLOTBASE_H

#include <QGraphicsObject>
#include "CustomAxis.h"
#include "Plot.h"

class PlotBase : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit PlotBase(const Plot &plot, QGraphicsItem *parent = nullptr);

    void setBoundingRect(const QRectF &rect);
    void setTitle(const QString &title);
    void setTitleFont(const QFont &font);
    void setAxisXName(const QString &name);
    void setAxisYName(const QString &name);

    int worksheetId() const { return m_plot.workSheetId();}
    int plotId() const {return m_plot.id();}
    QString title() const {return m_title;}
    QString axisXName() const {return m_xAxis->axisName();}
    QString axisYName() const {return m_yAxis->axisName();}
    int axisXSettingId() const {return m_plot.axisXId();}
    int axisYSettingId() const {return m_plot.axisYId();}
    PlotType plotType() const {return m_plot.plotType();}
    int axisXDetectorId() const {return m_plot.axisXDetectorId();}
    int axisYDetectorId() const {return m_plot.axisYDetectorId();}
    MeasurementType xMeasurementType() const {return m_plot.xMeasurementType();}
    MeasurementType yMeasurementType() const {return m_plot.yMeasurementType();}
    QRectF  plotArea() const {return m_plotArea;}

    qreal   mapValueToXAixs(qreal value) const;
    qreal   mapValueToYAixs(qreal value) const;
    qreal   mapXAxisToValue(qreal value) const;
    qreal   mapYAxisToValue(qreal value) const;

    QPointF mapPointToPlotArea(const QPointF &point) const;
    QPointF mapPointToPlotArea(qreal x, qreal y) const;
    QPointF mapPlotAreaToPoint(const QPointF &point) const;

    QRectF  mapRectToPlotArea(const QRectF &rect) const;
    QRectF  mapRectToAxis(const QRectF &rect) const;

    bool    isInPlotArea(const QPointF &point) const;
    QPointF limitPointInPlot(const QPointF &point) const;
    QPointF limitScenePointInPlot(const QPointF &point) const;

protected:
    void updateLayout();
    void paintTitle(QPainter *painter);;
    void paintAxis(QPainter *painter);

    virtual void paintPlot(QPainter *painter) = 0;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    // void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    // void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    // void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;


    Plot        m_plot;
    CustomAxis *m_xAxis;
    CustomAxis *m_yAxis;
    QString m_title;
    QRectF m_boundingRect;
    QRectF m_plotArea;
    QRectF m_axisXArea;
    QRectF m_axisYArea;
    QRectF m_titleArea;

    QFont m_titleFont;

    static constexpr qreal PLOT_MARGIN = 5.0;
    static constexpr qreal TITLE_MARGIN = 5.0;
};




#endif // PLOTBASE_H
