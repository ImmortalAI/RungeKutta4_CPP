#include "graphwindow.h"
#include "ui_graphwindow.h"

#include <QGraphicsLineItem>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>

GraphWindow::GraphWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphWindow) {
    ui->setupUi(this);

    connect(ui->exportButton, &QPushButton::clicked, this, &GraphWindow::onExportClicked);
}

GraphWindow::~GraphWindow() {
    delete ui;
}

void GraphWindow::plotGraph(const QVector<double>& x_vals, const QVector<double>& y_vals) {
    x_data = x_vals;
    y_data = y_vals;

    ui->graph->addGraph();
    ui->graph->graph(0)->setData(x_vals, y_vals);

    ui->graph->xAxis->setLabel("X");
    ui->graph->yAxis->setLabel("Y");

    double min_x = *std::min_element(x_data.begin(), x_data.end());
    double max_x = *std::max_element(x_data.begin(), x_data.end());
    double min_y = *std::min_element(y_data.begin(), y_data.end());
    double max_y = *std::max_element(y_data.begin(), y_data.end());
    if (max_y - min_y == 0) max_y += 1;

    ui->graph->xAxis->setRange(min_x, max_x);
    ui->graph->yAxis->setRange(min_y, min_y + (max_x - min_x) < max_y ? min_y + (max_x - min_x) : max_y);

    ui->graph->replot();
}

void GraphWindow::onExportClicked() {
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить CSV", "", "CSV файлы (*.csv)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "x,y\n";
        for (size_t i = 0; i < x_data.size(); ++i) {
            out << x_data[i] << "," << y_data[i] << "\n";
        }
        file.close();
    }
}
