#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QPen>

#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>

#include "exprtk.hpp"
#include "rk4.hpp"
#include "graphwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);

    connect(ui->evalButton, &QPushButton::clicked, this, &MainWindow::onCalculateClicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onCalculateClicked() {
    std::string expression_str = ui->diffInput->text().toStdString();
    double x0 = ui->initXInput->text().toDouble();

    // Read initial y0 and convert it to vector of double values
    std::string y0_str = ui->initYInput->toPlainText().toStdString();
    std::istringstream iss(y0_str);
    double y0_elem;
    std::vector<double> y0;
    while(iss >> y0_elem){
        y0.push_back(y0_elem);
    }

    double x_max = ui->toXInput->text().toDouble();
    double h = ui->stepInput->text().toDouble();

    RungeKutta4<double> rk4(expression_str, x0, y0, h);

    double prev_x = x0;
    auto prev_y = y0;

    QVector<double> x_vals, y_vals;
    x_vals.push_back(x0);
    y_vals.push_back(y0.at(0));

    while(rk4.getX() <= x_max){
        double next_y = rk4.makeStep();
        prev_x = rk4.getX();
        prev_y = rk4.getY();
        x_vals.push_back(prev_x);
        y_vals.push_back(next_y);
    }

    GraphWindow *window = new GraphWindow(this);
    window->plotGraph(x_vals, y_vals);
    window->exec();
}