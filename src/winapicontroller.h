#ifndef WINAPICONTROLLER_H
#define WINAPICONTROLLER_H

#include "abstractplatformcontroller.h"

class WinapiController : public AbstractPlatformController
{
public:
    void sendMouseButtonEvent(int button, bool down) override;
    void sendMouseWheelEvent(double x, double y) override;
    bool isHorizontalScrollAvailable() const override;
};

#endif // WINAPICONTROLLER_H
