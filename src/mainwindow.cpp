#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QPen>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->functionGraph->setScene(scene);

    connect(ui->funcEnterButton, &QPushButton::clicked, this, &MainWindow::onCalculateClicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onCalculateClicked() {
    scene->clear();

    std::string expression_str = ui->funcInputField->text().toStdString();
    double x0 = ui->funcInitInputField->text().toDouble();
    double y0 = ui->funcEqInitInputField->text().toDouble();
    double x_max = ui->toXInputField->text().toDouble();
    double h = ui->stepInputField->text().toDouble();

    RungeKutta4<double> rk4(expression_str, x0, y0, h);

    QPen pen(Qt::blue);
    double prev_x = x0, prev_y = y0;

    while(rk4.getX() <= x_max){
        double next_y = rk4.makeStep();
        scene->addLine(prev_x * 20, -prev_y * 20, rk4.getX() * 20, -next_y * 20, pen);
        prev_x = rk4.getX();
        prev_y = next_y;
    }
}