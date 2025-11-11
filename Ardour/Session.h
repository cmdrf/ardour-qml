#ifndef SESSION_H
#define SESSION_H

#include <QtQuick/QQuickPaintedItem>

class Session : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_DISABLE_COPY(Session)
public:
    explicit Session(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;
    ~Session() override;
};

#endif // SESSION_H
