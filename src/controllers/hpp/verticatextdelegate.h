#pragma once
#ifndef VERTICATEXTDELEGATE_H
#define VERTICATEXTDELEGATE_H

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

#endif // VERTICATEXTDELEGATE_H
