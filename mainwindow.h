#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <QMetaType>
#include <QPen>
#include "MyGraph.h"
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void getSignal1(trackcell trace,int color);

    void getSignal2(VexCell vex);

    void getSignal3(int dist);

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
    MyGraph* graph;
    QPixmap pix;
    QPushButton button[13];
};
#endif // MAINWINDOW_H
