#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "gamepad.h"
#include "abstractplatformcontroller.h"
#include <QTimer>
#include <QSettings>

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QSettings& settings, QObject *parent = nullptr);
    const QMap<int, Gamepad*>& getGamepads() const { return gamepads; }

signals:
    void gamepadsChanged();

private slots:
    void updateFrame();

    void updateGamepadsConnection();
    void onGamepadConnected(int deviceId);
    void onGamepadDisconnected(int deviceId);

private:
    QMap<int, Gamepad*> gamepads;
    QSettings& settings;
    QTimer timerUpdateFrame;
    AbstractPlatformController& platform;
};

#endif // CONTROLLER_H
