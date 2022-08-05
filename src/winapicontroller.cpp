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

}

WinapiController::WinapiController(QObject *parent)
    : AbstractPlatformController(parent)
{
#if _WIN32_WINNT < 0x0600
    qWarning() << "Horizontal scrolling is not supported for this target Windows version. Check https://docs.microsoft.com/en-us/cpp/porting/modifying-winver-and-win32-winnt?view=msvc-170";
#endif
}

void WinapiController::sendMouseButtonEvent(Qt::MouseButton button, bool down)
{
    POINT pos;
    if (!GetCursorPos(&pos))
    {
        printError(GetLastError());
    }

    DWORD dwFlags = 0;
    switch (button)
    {
    case Qt::LeftButton:
        dwFlags = down ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_LEFTUP;
        break;

    case Qt::RightButton:
        dwFlags = down ? MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_RIGHTUP;
        break;

    case Qt::MiddleButton:
        dwFlags = down ? MOUSEEVENTF_MIDDLEDOWN : MOUSEEVENTF_MIDDLEUP;
        break;

    default:
        qCritical() << "Unknown mouse button" << button;
        return;
    }

    mouse_event(dwFlags, pos.x, pos.y, 0, 0);
}

void WinapiController::sendMouseWheelEvent(const QPointF &speed)
{
    POINT pos;
    if (!GetCursorPos(&pos))
    {
        printError(GetLastError());
    }

#if _WIN32_WINNT >= 0x0600
    mouse_event(MOUSEEVENTF_HWHEEL, pos.x, pos.y, DWORD(WHEEL_DELTA * speed.x()), 0);
#endif

    mouse_event(MOUSEEVENTF_WHEEL, pos.x, pos.y, DWORD(-WHEEL_DELTA * speed.y()), 0);

}
