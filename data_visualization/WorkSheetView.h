#ifndef WORKSHEETVIEW_H
#define WORKSHEETVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QMouseEvent>


#include "WorkSheet.h"
#include "WorkSheetScene.h"
#include "PlotBase.h"


class WorkSheetView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit WorkSheetView(const WorkSheet &worksheet, QWidget *parent = nullptr);
    QString name() const;

    int  worksheetId() const { return m_workSheet.id(); }
    void updateData();
    WorkSheetScene *scene() const { return m_scene; }

// protected:
//     void wheelEvent(QWheelEvent *event) override;
//     void mousePressEvent(QMouseEvent *event) override;
//     void mouseMoveEvent(QMouseEvent *event) override;
//     void mouseReleaseEvent(QMouseEvent *event) override;

private:
    static constexpr int defaultUpdateInterval = 1000;
    WorkSheet           m_workSheet;
    WorkSheetScene      *m_scene;

    void initWorkSheetView();
};

inline QString WorkSheetView::name() const
{
    return m_workSheet.name();
}

#endif // WORKSHEETVIEW_H
