#include "linuxcontroller.h"
#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

// https://gist.github.com/pioz/726474

namespace
{

unsigned int intButtonToXEventButton(int button, bool& ok)
{
    ok = false;

    switch (button)
    {
    case AbstractPlatformController::LEFT_BUTTON:
        ok = true;
        return Button1;

    case AbstractPlatformController::MIDDLE_BUTTON:
        ok = true;
        return Button2;

    case AbstractPlatformController::RIGHT_BUTTON:
        ok = true;
        return Button3;

    default:
        break;
    }

    return 0;
}

}

LinuxController::LinuxController()
{
    display = XOpenDisplay(nullptr);
    if (!display)
    {
        fprintf(stderr, "Failed to open display\n");
    }
}

LinuxController::~LinuxController()
{
    if (display)
    {
        XCloseDisplay((Display*)display);
        display = nullptr;
    }
}

void LinuxController::sendMouseButtonEvent(int button_, bool down)
{
    if (!display)
    {
        fprintf (stderr, "Display not opened\n");
        return;
    }

    bool ok = false;
    const auto button = intButtonToXEventButton(button_, ok);
    if (!ok)
    {
        fprintf (stderr, "Failed to convert int mouse button to x11 mouse button\n");
        return;
    }

    const int ret = XTestFakeButtonEvent((Display*)display, button, down, CurrentTime);
    XFlush((Display*)display);
    if (ret != 1)
    {
        fprintf (stderr, "XTestFakeButtonEvent error %d\n", ret);
    }
}

void LinuxController::sendMouseWheelEvent(double, double y)
{
    if (skipWheelEventSteps > 0)
    {
        skipWheelEventSteps--;
        return;
    }

    if (!display)
    {
        fprintf (stderr, "Display not opened\n");
        return;
    }

    int button = 0;

    if (y > 0)
    {
        button = Button5;
    }
    else if (y < 0)
    {
        button = Button4;
    }
    else
    {
        return;
    }

    int ret = XTestFakeButtonEvent((Display*)display, button, true, CurrentTime);
    XFlush((Display*)display);
    if (ret != 1)
    {
        fprintf (stderr, "XTestFakeButtonEvent error %d\n", ret);
    }

    ret = XTestFakeButtonEvent((Display*)display, button, false, CurrentTime);
    XFlush((Display*)display);
    if (ret != 1)
    {
        fprintf (stderr, "XTestFakeButtonEvent error %d\n", ret);
    }

    skipWheelEventSteps = 10 - y;
}

bool LinuxController::isHorizontalScrollAvailable() const
{
    return false;
}
