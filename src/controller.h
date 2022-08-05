#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "gamepad.h"
#include "abstractplatformcontroller.h"
#include <QTimer>

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);

signals:

private slots:
    void updateFrame();

    void onGamepadConnected(int deviceId);
    void onGamepadDisconnected(int deviceId);

private:
    QMap<int, Gamepad*> gamepads;
    QTimer timerUpdateFrame;
    AbstractPlatformController& platform;
};

#endif // CONTROLLER_H
