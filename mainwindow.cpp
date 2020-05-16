#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    graph=new MyGraph;
    connect(graph,SIGNAL(Signal1(trackcell,int)),this,SLOT(getSignal1(trackcell,int)));
    connect(graph,SIGNAL(Signal2(VexCell)),this,SLOT(getSignal2(VexCell)));
    connect(graph,SIGNAL(Signal3(int)),this,SLOT(getSignal3(int)));
    pix=QPixmap(1300,850);
    pix.fill(Qt::transparent);
    graph->_initGraph("D:\\QTcode\\Demo\\BJTU_travel\\BJTU_travel\\vertex.txt", "D:\\QTcode\\Demo\\BJTU_travel\\BJTU_travel\\arc.txt");
    //graph->shortest_path(0,10);
    //graph.MSP_prim(0);
    //graph.paint(11,1,0);


}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.drawPixmap(0,0,1300,700,pix);

}

//最小生成树
void MainWindow::on_pushButton_clicked()
{
    QString vex=ui->lineEdit->text();
    qDebug()<<vex;
    int v=graph->locateVex(vex.toStdString());
    graph->MSP_prim(v);
}
//最短路径
void MainWindow::on_pushButton_2_clicked()
{
    QString vex1=ui->lineEdit_2->text();
    int v1=graph->locateVex(vex1.toStdString());
    QString vex2=ui->lineEdit_3->text();
    int v2=graph->locateVex(vex2.toStdString());
    graph->shortest_path(v1,v2);
}
//重新生成地图
void MainWindow::on_pushButton_3_clicked()
{
    graph->_initGraph("D:\\QTcode\\Demo\\BJTU_travel\\BJTU_travel\\vertex.txt", "D:\\QTcode\\Demo\\BJTU_travel\\BJTU_travel\\arc.txt");
}
//获得景点信息
void MainWindow::on_pushButton_4_clicked()
{
    QString vex1=ui->lineEdit_5->text();
    int v1=graph->locateVex(vex1.toStdString());
    ui->plainTextEdit->clear();
    ui->plainTextEdit->appendPlainText(QString::fromStdString(graph->getVexInf(v1)));
}
void MainWindow::getSignal1(trackcell trace,int color){
    QPainter painter(&pix);
    QPen pen(Qt::black,3,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
    if(color==0)
        pen.setColor(Qt::black);
    else
        pen.setColor(Qt::red);
    painter.setPen(pen);
    int x,y;
    x=trace.x1;
    y=trace.y1;//保存临时位置

    //绘制轨迹
    for(int i=0;i<trace.count;i++){
        if(trace.direction[i]=='s'){
            painter.drawLine(x , y , x , y+trace.dist[i]*2);
            y+=trace.dist[i]*2;
        }
        else if(trace.direction[i]=='n'){
            painter.drawLine(x , y , x , y-trace.dist[i]*2);
            y-=trace.dist[i]*2;
        }
        else if(trace.direction[i]=='e'){
            painter.drawLine(x , y , x+trace.dist[i]*2 , y);
            x+=trace.dist[i]*2;
        }
        else if(trace.direction[i]=='w'){
            painter.drawLine(x , y , x-trace.dist[i]*2 , y);
            x-=trace.dist[i]*2;
        }
        else
            cout<<"ERROR";
    }
    update();

}

void MainWindow::getSignal2(VexCell vex){
    int x=vex.x;
    int y=vex.y;
    QPainter painter(&pix);
    QPen pen(Qt::red,3,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
    //设置画笔等一系列设置
    painter.setPen(pen);
    QBrush brush(Qt::red);
    painter.setBrush(brush);
    painter.setRenderHint(QPainter::Antialiasing);
    //顶点画圆
    painter.drawEllipse(QPoint(x,y),6,6);
    update();
    //顶点文字
    QFont font("宋体",10,QFont::Bold,true);
    painter.setFont(font);
    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.drawText(QRect(x+5,y+5,x+100,y+100),QString::fromStdString(vex.name));
    //button[count].
}

void MainWindow::getSignal3(int dist){
    ui->lineEdit_4->setText(QString::number(dist));
}


