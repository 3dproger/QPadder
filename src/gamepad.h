#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <QPoint>
#include <QGamepad>
#include <QSettings>

class Gamepad : public QObject
{
    Q_OBJECT
public:
    enum class StandardButton { None, A, B, X, Y, L1, L2, L3, R1, R2, R3, Left, Right, Up, Down, Start, Select, Center, Guide };
    enum class StandardStick { None, Left, Right };

    struct Config
    {
        QMap<StandardButton, Qt::MouseButton> standardMouseConfig;
        StandardStick standardMouseMoveStick = StandardStick::None;
        StandardStick standardMouseScrollStick = StandardStick::None;

        static Config standardConfig()
        {
            Config config;

            config.standardMouseConfig.insert(StandardButton::A, Qt::MouseButton::LeftButton);
            config.standardMouseConfig.insert(StandardButton::R3, Qt::MouseButton::MiddleButton);
            config.standardMouseConfig.insert(StandardButton::B, Qt::MouseButton::RightButton);

            config.standardMouseMoveStick = StandardStick::Left;
            config.standardMouseScrollStick = StandardStick::Right;

            return config;
        }

        void load(QSettings& settings, const QString& groupPath);
    };

    explicit Gamepad(int deviceId, QObject *parent = nullptr);

    const QGamepad& getQGamepad() const { return gamepad; };

    QPoint getCursorSpeed() const;
    QPointF getWheelSpeed() const;

signals:
    void mouseButtonChanged(Qt::MouseButton button, bool down);

private slots:
    void onStandardButtonChanged(const StandardButton& button, bool down);

private:
    QPointF getLeftAxisDeadZoned() const;
    QPointF getRightAxisDeadZoned() const;

    QGamepad gamepad;

    const double leftDeadZone = 0.5;
    const double rightDeadZone = 0.75;

    const double cursorSensitivityByAxis = 7;
    const double cursorSensitivityByArrow = 5;

    const double wheelSensitivity = 0.1;

    Config config = Config::standardConfig();
};

#endif // GAMEPAD_H
