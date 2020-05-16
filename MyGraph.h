#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>
#include <QObject>
#include <QDebug>
#include <QString>

#define MAX_VERTEX_NUM 20
#define INFINITY 65535

using namespace std;

typedef struct ArcCell {
    int distance;//距离
    string track;//轨迹
}AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

typedef struct VexCell {
    int data;//编号
    string name;//名称
    int x;//横坐标
    int y;//纵坐标
    string introduction;//简介
}vertexs[MAX_VERTEX_NUM];

typedef struct closecell{
    int adjvex;//  集合U里面的顶点
    int lowcost;//权值
}closedge[MAX_VERTEX_NUM];

struct trackcell{
    int x1;
    int y1;
    int x2;
    int y2;
    char* direction;
    int* dist;
    int count;
    trackcell(int _x1,int _y1,int _x2,int _y2,char* _direction,int* _dist,int _count)
        :x1(_x1),y1(_y1),x2(_x2),y2(_y2),direction(_direction),dist(_dist),count(_count){}
};//一条路径的信息

class MyGraph :public QObject{
    Q_OBJECT
private:
    int _vexNum;//顶点数量
    int _arcNum;//边数量
    AdjMatrix _arcs;//边的邻接矩阵
    vertexs _vex;//顶点集

public:
    MyGraph();
    ~MyGraph();
    bool _initGraph(const char* vexFile, const char* arcFile);//从文件读取点和边的信息
    void paint(int v0,int v1,int color);//把v0和v1的点连起来 color 0代表黑色 1代表红色 2代表绿色
    void shortest_path(int v0,int v1);//获得 v0 v1 之间的最短路径
    void MSP_prim(int v);//通过prim算法获得最小生成树
    int locateVex(string name);//获得顶点的位置
    string getVexInf(int v);//获得景点的介绍
private:
    void _paintVex(VexCell vex);
    bool _getVexs(const char* filename);//从文件获得顶点集和相关数据
    bool _getArcs(const char* filename);//从文件获得边集合相关数据
    void _shortest_path_DIj(int v0, int* dist, int* path);//获得指定点和其他各点的最短路径 dist代表距离  path存储路径
    int _minimun_closedge(closedge a);//找到closedge中的最小权值点
    void _divide_trace(int v0,int v1,char* direction, int* sub_dist,int& count);//分离trace字符串 获得轨迹
    string UTF8ToGB(const char* str);//解决中文乱码问题
signals:
    void Signal1(trackcell _track,int color);//轨迹信号
    //void Signal2(int x,int y);
    void Signal2(VexCell vex);//顶点信号
    void Signal3(int dist);//距离
};
