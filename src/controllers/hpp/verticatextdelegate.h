#ifndef VERTICATEXTDELEGATE_H
#define VERTICATEXTDELEGATE_H

#include <QtWidgets>

class verticalTextDelegate : public QStyledItemDelegate
{
public:
    explicit verticalTextDelegate(QObject *parent = nullptr);
    ~verticalTextDelegate() = default;

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};

#endif // VERTICATEXTDELEGATE_H
