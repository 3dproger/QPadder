#ifndef LINUXCONTROLLER_H
#define LINUXCONTROLLER_H

#include "abstractplatformcontroller.h"

class LinuxController : public AbstractPlatformController
{
    Q_OBJECT
public:
    explicit LinuxController(QObject *parent = nullptr);
    void sendMouseButtonEvent(Qt::MouseButton button, bool down) override;
    void sendMouseWheelEvent(const QPointF &speed) override;

signals:

};

#endif // LINUXCONTROLLER_H
