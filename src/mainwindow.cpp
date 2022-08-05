#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->statusBar->addWidget(labelStatus);

    connect(&controller, &Controller::gamepadsChanged, this, &MainWindow::onGamepadsChanged);

    onGamepadsChanged();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onGamepadsChanged()
{
    const QMap<int, Gamepad*>& gamepads = controller.getGamepads();
    labelStatus->setText(tr("Connected gamepads: %1").arg(gamepads.count()));
}

