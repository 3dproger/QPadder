#include "gamepad.h"
#include <cmath>

namespace
{

}

Gamepad::Gamepad(int deviceId, QObject *parent)
    : QObject{parent}
    , gamepad(deviceId)
{
    connect(&gamepad, &QGamepad::buttonAChanged, this, [this](bool value)
    {
        emit mouseButtonChanged(Qt::MouseButton::LeftButton, value);
    });

    connect(&gamepad, &QGamepad::buttonL3Changed, this, [this](bool value)
    {
        emit mouseButtonChanged(Qt::MouseButton::LeftButton, value);
    });

    connect(&gamepad, &QGamepad::buttonR3Changed, this, [this](bool value)
    {
        emit mouseButtonChanged(Qt::MouseButton::MiddleButton, value);
    });

    connect(&gamepad, &QGamepad::buttonBChanged, this, [this](bool value)
    {
        emit mouseButtonChanged(Qt::MouseButton::RightButton, value);
    });
}

QPoint Gamepad::getCursorSpeed() const
{
    QPointF p = getLeftAxisDeadZoned() * cursorSensitivityByAxis;

    if (gamepad.buttonLeft())
    {
        p.rx() -= cursorSensitivityByArrow;
    }

    if (gamepad.buttonRight())
    {
        p.rx() += cursorSensitivityByArrow;
    }

    if (gamepad.buttonUp())
    {
        p.ry() -= cursorSensitivityByArrow;
    }

    if (gamepad.buttonDown())
    {
        p.ry() += cursorSensitivityByArrow;
    }

    return p.toPoint();
}

QPointF Gamepad::getWheelSpeed() const
{
    return getRightAxisDeadZoned() * wheelSensitivity;
}

QPointF Gamepad::getLeftAxisDeadZoned() const
{
    QPointF p = QPointF(gamepad.axisLeftX(), gamepad.axisLeftY());
    if (fabs(p.x()) < leftDeadZone && fabs(p.y()) < leftDeadZone)
    {
        return QPointF();
    }

    return p;
}

QPointF Gamepad::getRightAxisDeadZoned() const
{
    QPointF p = QPointF(gamepad.axisRightX(), gamepad.axisRightY());
    if (fabs(p.x()) < rightDeadZone && fabs(p.y()) < rightDeadZone)
    {
        return QPointF();
    }

    return p;
}
