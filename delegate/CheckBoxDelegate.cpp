#include "CheckBoxDelegate.h"
#include <QApplication>


CheckBoxDelegate::CheckBoxDelegate(QObject *parent)
: QStyledItemDelegate{parent}
{

}

QWidget *CheckBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QCheckBox *checkBox = new QCheckBox(parent);
    checkBox->setAutoFillBackground(true);
    checkBox->setTristate(false);
    return checkBox;
}


void CheckBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
    if (!checkBox) {
        return;
    }
    bool value = index.model()->data(index, Qt::EditRole).toBool();
    checkBox->setChecked(value);
}

void CheckBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
    if (!checkBox) {
        return;
    }
    model->setData(index, checkBox->isChecked(), Qt::EditRole);
}


void CheckBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void CheckBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    bool value = index.model()->data(index, Qt::EditRole).toBool();

    QStyleOptionButton checkboxOption;
    checkboxOption.state = value ? QStyle::State_On : QStyle::State_Off;
    checkboxOption.state |= QStyle::State_Enabled;
    checkboxOption.rect = option.rect;
    checkboxOption.text = value ? "Enabled" : "Disabled";

    QApplication::style()->drawControl(QStyle::CE_CheckBox, &checkboxOption, painter);
}
