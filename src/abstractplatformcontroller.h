#ifndef ABSTRACTPLATFORMCONTROLLER_H
#define ABSTRACTPLATFORMCONTROLLER_H

class AbstractPlatformController
{
public:
    static const int LEFT_BUTTON = 0;
    static const int MIDDLE_BUTTON = 1;
    static const int RIGHT_BUTTON = 2;

    virtual void sendMouseButtonEvent(int button, bool down) = 0;
    virtual void sendMouseWheelEvent(double x, double y) = 0;
    virtual bool isHorizontalScrollAvailable() const = 0;
};

#endif // ABSTRACTPLATFORMCONTROLLER_H
