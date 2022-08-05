#include "controller.h"
#ifdef Q_OS_WIN
#include "winapicontroller.h"
#endif
#include <QGamepadManager>
#include <QCursor>
#include <QDebug>
#include <QMouseEvent>
#include <QApplication>

Controller::Controller(QObject *parent)
    : QObject{parent}
#ifdef Q_OS_WIN
    , platform(*new WinapiController())
#endif
{
    connect(QGamepadManager::instance(), &QGamepadManager::gamepadConnected, this, &Controller::onGamepadConnected);
    connect(QGamepadManager::instance(), &QGamepadManager::gamepadDisconnected, this, &Controller::onGamepadDisconnected);

    timerUpdateFrame.setInterval(16);
    connect(&timerUpdateFrame, &QTimer::timeout, this, &Controller::updateFrame);
    timerUpdateFrame.start();
}

void Controller::updateFrame()
{
    for (auto it = gamepads.begin(); it != gamepads.end(); ++it)
    {
        const QPoint cursorSpeed = it.value()->getCursorSpeed();
        if (cursorSpeed.manhattanLength() != 0)
        {
            QCursor::setPos(QCursor::pos() + cursorSpeed);
        }

        const QPointF wheelSpeed = it.value()->getWheelSpeed();
        if (wheelSpeed.manhattanLength() != 0.0)
        {
            platform.sendMouseWheelEvent(wheelSpeed);
        }
    }
}

void Controller::onGamepadConnected(int deviceId)
{
    if (gamepads.contains(deviceId))
    {
        qCritical() << "Gamepad " << deviceId << " already found in connected";
        return;
    }

    Gamepad* gamepad = new Gamepad(deviceId, this);
    gamepads.insert(deviceId, gamepad);

    qDebug() << "Connected" << deviceId << gamepad->getQGamepad().name();

    connect(gamepad, &Gamepad::mouseButtonChanged, this, [this](Qt::MouseButton button, bool down)
    {
        platform.sendMouseButtonEvent(button, down);
    });
}

void Controller::onGamepadDisconnected(int deviceId)
{
    if (!gamepads.contains(deviceId))
    {
        qCritical() << "Gamepad " << deviceId << " not found in connected";
        return;
    }

    Gamepad* gamepad = gamepads.value(deviceId);

    const QString name = gamepad->getQGamepad().name();

    gamepads.remove(deviceId);
    gamepad->deleteLater();

    qDebug() << "Disconnected" << deviceId << name;
}
