#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "controller.h"
#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onGamepadsChanged();

private:
    Ui::MainWindow *ui;
    QLabel* labelStatus = new QLabel(this);
    QSettings settings;
    Controller* controller = nullptr;
};
#endif // MAINWINDOW_H
