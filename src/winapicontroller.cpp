#include "winapicontroller.h"
#include "targetver.h"
#include <windows.h>
#include <winuser.h>
#include <QDebug>

namespace
{

void printError(DWORD error)
{
    // https://stackoverflow.com/questions/1387064/how-to-get-the-error-message-from-the-error-code-returned-by-getlasterror

    if (error == 0)
    {
        qCritical() << "No winapi error";
    }

    LPWSTR messageBuffer = nullptr;
    const size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, NULL);
    if (!messageBuffer)
    {
        qCritical() << "Winapi error:" << error << ", but failed to get error string";
        return;
    }

    QString errorString = QString::fromWCharArray(messageBuffer, size);
    if (errorString.endsWith("\r\n"))
    {
        errorString = errorString.left(errorString.length() - 2);
    }

    qCritical() << "Error" << error << ":" << errorString;

    LocalFree(messageBuffer);
}

DWORD intButtonToWinapiMouseButton(int button, bool down, bool& ok)
{
    ok = false;

    switch (button)
    {
    case AbstractPlatformController::LEFT_BUTTON:
        ok = true;
        return down ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_LEFTUP;

    case AbstractPlatformController::MIDDLE_BUTTON:
        ok = true;
        return down ? MOUSEEVENTF_MIDDLEDOWN : MOUSEEVENTF_MIDDLEUP;

    case AbstractPlatformController::RIGHT_BUTTON:
        ok = true;
        return down ? MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_RIGHTUP;

    default:
        break;
    }

    return 0;
}

}

void WinapiController::sendMouseButtonEvent(int button, bool down)
{
    POINT pos;
    if (!GetCursorPos(&pos))
    {
        printError(GetLastError());
    }

    bool ok = false;
    DWORD dwFlags = intButtonToWinapiMouseButton(button, down, ok);
    if (!ok)
    {
        qCritical() << "Failed to conver int mouse button to winapi mouse button";
        return;
    }

    mouse_event(dwFlags, pos.x, pos.y, 0, 0);
}

void WinapiController::sendMouseWheelEvent(double x, double y)
{
    POINT pos;
    if (!GetCursorPos(&pos))
    {
        printError(GetLastError());
    }

#if defined(MOUSEEVENTF_HWHEEL)
    mouse_event(MOUSEEVENTF_HWHEEL, pos.x, pos.y, DWORD(WHEEL_DELTA * x), 0);
#endif

    mouse_event(MOUSEEVENTF_WHEEL, pos.x, pos.y, DWORD(-WHEEL_DELTA * y), 0);

}

bool WinapiController::isHorizontalScrollAvailable() const
{
#if defined(MOUSEEVENTF_HWHEEL)
    return true;
#else
    return false;
#endif
}
