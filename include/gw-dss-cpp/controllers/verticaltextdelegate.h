#pragma once
#ifndef VERTICALTEXTDELEGATE_H
#define VERTICALTEXTDELEGATE_H

#include <QtWidgets>

class VerticalTextDelegate : public QStyledItemDelegate
{
public:
    explicit VerticalTextDelegate(QObject *parent = nullptr);
    ~VerticalTextDelegate() = default;

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option,
                       const QModelIndex &index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem &option,
                           const QModelIndex &index) const override;
};

#endif // VERTICALTEXTDELEGATE_H
