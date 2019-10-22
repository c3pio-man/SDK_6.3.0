#ifndef DOTTEDSEPARATOR_H
#define DOTTEDSEPARATOR_H

#include "qtpb_global.h"

#include <QObject>
#include <QPainter>
#include <QQuickItem>
#include <QQuickPaintedItem>

class QTPBSHARED_EXPORT DottedSeparator : public QQuickPaintedItem
{
    Q_OBJECT
    Q_DISABLE_COPY(DottedSeparator)

    QColor color_;

public:
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    DottedSeparator(QQuickItem *parent = 0);
    ~DottedSeparator();
    QColor color() const;

    void paint(QPainter *painter) override;
public slots:
    void setColor(QColor color);
signals:
    void colorChanged(QColor color);
};

#endif // DOTTEDSEPARATOR_H
