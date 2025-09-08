#ifndef TUBEBUTTONDELEGATE_H
#define TUBEBUTTONDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

class TubeButtonDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TubeButtonDelegate(QObject *parent = nullptr);


    void paint(QPainter *p, const QStyleOptionViewItem &opt, const QModelIndex &index) const override;

    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &, const QModelIndex &index) override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

signals:
    void tubeSelected(int tubeId);

private:
    QRect getButtonRect(const QStyleOptionViewItem &option) const;
};

#endif // TUBEBUTTONDELEGATE_H
