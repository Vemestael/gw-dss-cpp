#include "../hpp/verticatextdelegate.h"

verticalTextDelegate::verticalTextDelegate(QObject *parent)
    : QStyledItemDelegate(parent){};

void verticalTextDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                   const QModelIndex &index) const
{
    QStyleOptionViewItem optionCopy = option;
    QPointF rectCenter = QRectF(option.rect).center();
    painter->save();
    painter->translate(rectCenter.x(), rectCenter.y());
    painter->rotate(-90.0);
    painter->translate(-rectCenter.x(), -rectCenter.y());
    optionCopy.rect = painter->worldTransform().mapRect(option.rect);
    QStyledItemDelegate::paint(painter, optionCopy, index);
    painter->restore();
}

QSize verticalTextDelegate::sizeHint(const QStyleOptionViewItem &option,
                       const QModelIndex &index) const {
    QSize val = QStyledItemDelegate::sizeHint(option, index);
    return QSize(val.height(), val.width());
}

