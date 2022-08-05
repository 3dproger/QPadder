#ifndef LINUXCONTROLLER_H
#define LINUXCONTROLLER_H

#include "abstractplatformcontroller.h"

class LinuxController : public AbstractPlatformController
{
public:
    LinuxController();
    ~LinuxController();
    void sendMouseButtonEvent(int button, bool down) override;
    void sendMouseWheelEvent(double x, double y) override;
    bool isHorizontalScrollAvailable() const override;

private:
    void* display = nullptr;
    int skipWheelEventSteps = 0;
};

#endif // LINUXCONTROLLER_H
