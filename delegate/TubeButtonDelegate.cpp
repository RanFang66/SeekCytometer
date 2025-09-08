#include "TubeButtonDelegate.h"
#include <QApplication>
#include "BrowserDataModel.h"
#include <QPainter>

TubeButtonDelegate::TubeButtonDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}

void TubeButtonDelegate::paint(QPainter *p, const QStyleOptionViewItem &opt, const QModelIndex &index) const
{
    bool isTube = index.data(BrowserDataModel::IsTubeRole).toBool();
    if (isTube) {
        bool isSelected = index.data(BrowserDataModel::IsSelectedRole).toBool();

        QRect indicatorRect = opt.rect;
        indicatorRect.setWidth(16); // 指示灯宽度
        indicatorRect.setHeight(16); // 指示灯高度
        indicatorRect.moveCenter(opt.rect.center());
        indicatorRect.moveLeft(opt.rect.left() + 2); // 左侧留2像素边距

        // 根据选择状态设置颜色
        QColor indicatorColor;
        if (isSelected) {
            indicatorColor = Qt::green; // 选中状态为绿色
        } else {
            indicatorColor = Qt::gray;  // 未选中状态为灰色
        }

        p->save();
        p->setRenderHint(QPainter::Antialiasing);
        p->setBrush(indicatorColor);
        p->setPen(Qt::NoPen);
        p->drawEllipse(indicatorRect);
        p->restore();
    }
}

bool TubeButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonRelease && index.data(BrowserDataModel::IsTubeRole).toBool()) {
        model->setData(index, true, BrowserDataModel::IsSelectedRole);
        return true;
    }
    return false;
}

QSize TubeButtonDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(30, option.rect.height());
}

QRect TubeButtonDelegate::getButtonRect(const QStyleOptionViewItem &option) const
{
    QRect rect = option.rect;
    int size = qMin(rect.width(), rect.height()) - 4; // 留边距
    return QRect(rect.left() + (rect.width() - size) / 2,
                 rect.top() + (rect.height() - size) / 2,
                 size, size);
}
