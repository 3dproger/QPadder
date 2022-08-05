#include "gamepad.h"
#include <cmath>

namespace
{

QString StandardButtonToStr(Gamepad::StandardButton b)
{
    switch (b)
    {
    case Gamepad::StandardButton::None: return QString();
    case Gamepad::StandardButton::A: return "A";
    case Gamepad::StandardButton::B: return "B";
    case Gamepad::StandardButton::X: return "X";
    case Gamepad::StandardButton::Y: return "Y";
    case Gamepad::StandardButton::L1: return "L1";
    case Gamepad::StandardButton::L2: return "L2";
    case Gamepad::StandardButton::L3: return "L3";
    case Gamepad::StandardButton::R1: return "R1";
    case Gamepad::StandardButton::R2: return "R2";
    case Gamepad::StandardButton::R3: return "R3";
    case Gamepad::StandardButton::Left: return "LEFT";
    case Gamepad::StandardButton::Right: return "RIGHT";
    case Gamepad::StandardButton::Up: return "UP";
    case Gamepad::StandardButton::Down: return "DOWN";
    case Gamepad::StandardButton::Start: return "START";
    case Gamepad::StandardButton::Select: return "SELECT";
    case Gamepad::StandardButton::Center: return "CENTER";
    case Gamepad::StandardButton::Guide: return "GUIDE";
    }

    return QString();
}

Gamepad::StandardButton StandardButtonFromStr(const QString& s)
{
    if (s == "A") return Gamepad::StandardButton::A;
    else if (s == "B") return Gamepad::StandardButton::B;
    else if (s == "X") return Gamepad::StandardButton::X;
    else if (s == "Y") return Gamepad::StandardButton::Y;

    return Gamepad::StandardButton::None;
}

}

Gamepad::Gamepad(int deviceId, QObject *parent)
    : QObject{parent}
    , gamepad(deviceId)
{
    connect(&gamepad, &QGamepad::buttonAChanged, this, [this](bool value){ onStandardButtonChanged(StandardButton::A, value); });
    connect(&gamepad, &QGamepad::buttonBChanged, this, [this](bool value){ onStandardButtonChanged(StandardButton::B, value); });
    connect(&gamepad, &QGamepad::buttonXChanged, this, [this](bool value){ onStandardButtonChanged(StandardButton::X, value); });
    connect(&gamepad, &QGamepad::buttonYChanged, this, [this](bool value){ onStandardButtonChanged(StandardButton::Y, value); });

    connect(&gamepad, &QGamepad::buttonL1Changed, this, [this](bool value){ onStandardButtonChanged(StandardButton::L1, value); });
    connect(&gamepad, &QGamepad::buttonL2Changed, this, [this](bool value){ onStandardButtonChanged(StandardButton::L2, value); });
    connect(&gamepad, &QGamepad::buttonL3Changed, this, [this](bool value){ onStandardButtonChanged(StandardButton::L3, value); });

    connect(&gamepad, &QGamepad::buttonR1Changed, this, [this](bool value){ onStandardButtonChanged(StandardButton::R1, value); });
    connect(&gamepad, &QGamepad::buttonR2Changed, this, [this](bool value){ onStandardButtonChanged(StandardButton::R2, value); });
    connect(&gamepad, &QGamepad::buttonR3Changed, this, [this](bool value){ onStandardButtonChanged(StandardButton::R3, value); });

    connect(&gamepad, &QGamepad::buttonLeftChanged, this, [this](bool value){ onStandardButtonChanged(StandardButton::Left, value); });
    connect(&gamepad, &QGamepad::buttonRightChanged, this, [this](bool value){ onStandardButtonChanged(StandardButton::Right, value); });
    connect(&gamepad, &QGamepad::buttonUpChanged, this, [this](bool value){ onStandardButtonChanged(StandardButton::Up, value); });
    connect(&gamepad, &QGamepad::buttonDownChanged, this, [this](bool value){ onStandardButtonChanged(StandardButton::Down, value); });

    connect(&gamepad, &QGamepad::buttonStartChanged, this, [this](bool value){ onStandardButtonChanged(StandardButton::Start, value); });
    connect(&gamepad, &QGamepad::buttonSelectChanged, this, [this](bool value){ onStandardButtonChanged(StandardButton::Select, value); });
    connect(&gamepad, &QGamepad::buttonCenterChanged, this, [this](bool value){ onStandardButtonChanged(StandardButton::Center, value); });
    connect(&gamepad, &QGamepad::buttonGuideChanged, this, [this](bool value){ onStandardButtonChanged(StandardButton::Guide, value); });
}

QPoint Gamepad::getCursorSpeed() const
{
    QPointF p;

    switch (config.standardMouseMoveStick)
    {
    case Gamepad::StandardStick::None:
        break;

    case Gamepad::StandardStick::Left:
        p += getLeftAxisDeadZoned() * cursorSensitivityByAxis;
        break;

    case Gamepad::StandardStick::Right:
        p += getRightAxisDeadZoned() * cursorSensitivityByAxis;
        break;
    }

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
    QPointF p;

    switch (config.standardMouseScrollStick)
    {
    case Gamepad::StandardStick::None:
        break;

    case Gamepad::StandardStick::Left:
        p += getLeftAxisDeadZoned() * wheelSensitivity;
        break;

    case Gamepad::StandardStick::Right:
        p += getRightAxisDeadZoned() * wheelSensitivity;
        break;
    }

    return p;
}

void Gamepad::onStandardButtonChanged(const StandardButton& button, bool down)
{
    if (config.standardMouseConfig.contains(button))
    {
        emit mouseButtonChanged(config.standardMouseConfig.value(button), down);
    }
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

void Gamepad::Config::load(QSettings &settings, const QString &groupPath)
{
    *this = standardConfig();

    standardMouseConfig.clear();

    standardMouseConfig.insert(StandardButtonFromStr(settings.value(groupPath + "/standard/mouse/left_button").toString()), Qt::MouseButton::LeftButton);
    standardMouseConfig.insert(StandardButtonFromStr(settings.value(groupPath + "/standard/mouse/middle_button").toString()), Qt::MouseButton::MiddleButton);
    standardMouseConfig.insert(StandardButtonFromStr(settings.value(groupPath + "/standard/mouse/right_button").toString()), Qt::MouseButton::RightButton);
}
