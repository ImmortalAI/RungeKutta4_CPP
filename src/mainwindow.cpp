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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);

    
    // scene = new QGraphicsScene(this);
    // ui->functionGraph->setScene(scene);

    connect(ui->evalButton, &QPushButton::clicked, this, &MainWindow::onCalculateClicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onCalculateClicked() {
    // scene->clear();

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

    // QPen pen(Qt::blue);
    double prev_x = x0;
    auto prev_y = y0;

    while(rk4.getX() <= x_max){
        double next_y = rk4.makeStep();
        // scene->addLine(prev_x * 20, -prev_y * 20, rk4.getX() * 20, -next_y * 20, pen);
        std::cout << prev_x << " " << next_y << std::endl;
        prev_x = rk4.getX();
        prev_y = rk4.getY();
    }
}