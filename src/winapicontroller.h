#ifndef WINAPICONTROLLER_H
#define WINAPICONTROLLER_H

#include "abstractplatformcontroller.h"

class WinapiController : public AbstractPlatformController
{
    Q_OBJECT
public:
    explicit WinapiController(QObject *parent = nullptr);
    void sendMouseButtonEvent(Qt::MouseButton button, bool down) override;
    void sendMouseWheelEvent(const QPointF &speed) override;

signals:

};

#endif // WINAPICONTROLLER_H
