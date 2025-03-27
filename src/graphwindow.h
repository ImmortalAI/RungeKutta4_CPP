#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QDialog>
#include <QGraphicsScene>
#include <QVector>
#include <vector>
#include "qcustomplot.h"

namespace Ui {
class GraphWindow;
}

class GraphWindow : public QDialog {
    Q_OBJECT

public:
    explicit GraphWindow(QWidget *parent = nullptr);
    ~GraphWindow();

    void plotGraph(const QVector<double>& x_vals, const QVector<double>& y_vals); 

private slots:
    void onExportClicked();

private:
    Ui::GraphWindow *ui;
    QVector<double> x_data, y_data;
};

#endif // GRAPHWINDOW_H
