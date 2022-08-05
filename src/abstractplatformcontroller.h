#ifndef ABSTRACTPLATFORMCONTROLLER_H
#define ABSTRACTPLATFORMCONTROLLER_H

#include <QObject>
#include <QPointF>

class AbstractPlatformController : public QObject
{
    Q_OBJECT
public:
    explicit AbstractPlatformController(QObject *parent = nullptr);
    virtual void sendMouseButtonEvent(Qt::MouseButton button, bool down) = 0;
    virtual void sendMouseWheelEvent(const QPointF& speed) = 0;

signals:

};

#endif // ABSTRACTPLATFORMCONTROLLER_H
