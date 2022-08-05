#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <QPoint>
#include <QGamepad>

class Gamepad : public QObject
{
    Q_OBJECT
public:
    explicit Gamepad(int deviceId, QObject *parent = nullptr);

    const QGamepad& getQGamepad() const { return gamepad; };

    QPoint getCursorSpeed() const;
    QPointF getWheelSpeed() const;

signals:
    void mouseButtonChanged(Qt::MouseButton button, bool down);

private:
    QPointF getLeftAxisDeadZoned() const;
    QPointF getRightAxisDeadZoned() const;

    QGamepad gamepad;

    const double leftDeadZone = 0.5;
    const double rightDeadZone = 0.75;

    const double cursorSensitivityByAxis = 7;
    const double cursorSensitivityByArrow = 5;

    const double wheelSensitivity = 0.1;
};

#endif // GAMEPAD_H
