#include "linuxcontroller.h"
#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>

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

    XEvent event;
    memset (&event, 0, sizeof (event));
    event.xbutton.button = button;
    event.xbutton.same_screen = true;
    event.xbutton.subwindow = DefaultRootWindow(display);
    while (event.xbutton.subwindow)
    {
        event.xbutton.window = event.xbutton.subwindow;
        XQueryPointer((Display*)display, event.xbutton.window,
            &event.xbutton.root, &event.xbutton.subwindow,
            &event.xbutton.x_root, &event.xbutton.y_root,
            &event.xbutton.x, &event.xbutton.y,
            &event.xbutton.state);
    }

    event.type = down ? ButtonPress : ButtonRelease;
    if (XSendEvent((Display*)display, PointerWindow, True, down ? ButtonPressMask : ButtonReleaseMask, &event) == 0)
    {
        fprintf (stderr, "Error to send the event!\n");
    }

    XFlush((Display*)display);
}

void LinuxController::sendMouseWheelEvent(double x, double y)
{
    if (!display)
    {
        fprintf (stderr, "Display not opened\n");
        return;
    }

}
