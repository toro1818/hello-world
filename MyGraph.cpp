#include "MyGraph.h"

MyGraph::MyGraph()
{
    _vexNum = 0;
    _arcNum = 0;
}

MyGraph::~MyGraph()
{
}
//获得顶点的位置
int MyGraph::locateVex(string name){
    for(int i=0;i<_vexNum;i++){
        if(name==_vex[i].name)
            return i;
    }
}
//获得景点的介绍
string MyGraph::getVexInf(int v){
    return _vex[v].introduction;
}
//绘制两点间线段
void MyGraph::paint(int v0,int v1,int color)
{
    //由于txt文件中轨迹是从小点到大点的，所以这里处理一下
    if (v0>v1) {
       int temp = v1;
       v1 = v0;
       v0 = temp;
    }
    char direction[5] = { "0000" };
    int sub_dist[4] = { 0 };
    int count = 0;
    _divide_trace(v0, v1, direction, sub_dist, count);
    trackcell _track(_vex[v0].x,_vex[v0].y,_vex[v1].x,_vex[v1].y,direction,sub_dist,count);
    if(color==0)
        emit Signal1(_track,0);
    else
        emit Signal1(_track,1);


}
void MyGraph::shortest_path(int v0, int v1)
{
    int* dist=new int[_vexNum];
    int* path = new int[_vexNum];
    _shortest_path_DIj(v0, dist, path);//获得最短路径
    emit Signal3(dist[v1]);//发送最短路径长度
    int temp = v1;
    while (temp != v0) {
        paint(temp,path[temp],1);//绘制两点之间的线段
        temp = path[temp];
    }
    delete[] dist;
    delete[] path;

}

void MyGraph::MSP_prim(int v)
{
    closedge _closedge;
    //初始化closedge
    for (int i = 0; i < _vexNum; ++i) {
        if (i != v) {
            _closedge[i].adjvex = v;
            _closedge[i].lowcost= _arcs[v][i].distance;
        }

    }
    //获得最小生成树
    _closedge[v].lowcost = 0;//U ={ u }
    int min;
    for (int j = 1; j < _vexNum; ++j) {
        min = _minimun_closedge(_closedge);
        paint(_closedge[min].adjvex,min,1);//在两点间画红色线
        _closedge[min].lowcost = 0;//把min点加入到U
        //更新closedge
        for (int k = 0; k < _vexNum; ++k) {
            if (_arcs[min][k].distance < _closedge[k].lowcost) {
                _closedge[k].adjvex = min;
                _closedge[k].lowcost= _arcs[min][k].distance ;
            }
        }
    }

}

//从文件读取点和边的信息
bool MyGraph::_initGraph(const char* vexFile, const char* arcFile)
{
    if (!_getVexs(vexFile))
        return false;
    if (!_getArcs(arcFile))
        return false;
    for(int i=0;i<_vexNum;i++)
        _paintVex(_vex[i]);
    return true;
}
//从文件获得顶点集和相关数据
bool MyGraph::_getVexs(const char* filename)
{
    ifstream infile(filename, ios::in);

    if (!infile.is_open()) {
        cout << "File failed to be opened.Filename:" << filename << endl;
        return false;
    }
    int count = 0;
    while (!infile.eof()) {
        infile >> _vex[count].data >> _vex[count].name >> _vex[count].x >> _vex[count].y >> _vex[count].introduction;        
        count++;
    }
    _vexNum = count;
    infile.close();
    return true;
}
//从文件获得边集合相关数据
bool MyGraph::_getArcs(const char* filename)
{
    ifstream infile(filename, ios::in);

    if (!infile.is_open()) {
        cout << "File failed to be opened.Filename:"<<filename << endl;
        return false;
    }
    int count = 0;
    char useless[255];
    while (!infile.eof()) {
        infile.getline(useless,255);
        count++;
    }
    _arcNum = count;//获得边数
    //初始化边
    for (int i = 0; i < _vexNum; i++) {
        for (int j = 0; j < _vexNum; j++) {
            if (i == j)
                _arcs[i][j].distance = 0;
            else
                _arcs[i][j].distance = INFINITY;
        }
    }
    infile.seekg(0, ios::beg);
    int tail=-1;
    int head = -1;
    int distance;
    string track;
    while (!infile.eof()) {
        infile >> tail >> head >> distance>>track;
        _arcs[tail][head].distance= distance;
        _arcs[tail][head].track = track;
        _arcs[head][tail].distance = distance;
        _arcs[head][tail].track = "0";//反向标志设为0，这里只使用从小点到大点的轨迹
        this->paint(tail,head,0);

    }
    infile.close();    
    return true;
}
void MyGraph::_paintVex(VexCell vex){
    emit Signal2(vex);
}
void MyGraph::_shortest_path_DIj(int v0, int* dist, int* path)
{
    bool* s = new bool[_vexNum];//判定是否被访问
    //初始化
    for (int i = 0; i < _vexNum; ++i) {
        dist[i] = _arcs[v0][i].distance;
        s[i] = false;
        if (dist[i] < INFINITY && i != v0)
            path[i] = v0;
        else
            path[i] = -1;
    }
    s[v0] = true;
    //找最短路径
    int min;
    int w;
    for (int i = 1; i < _vexNum; ++i) {//循环n-1次
        min = INFINITY;
        int u = v0;//u为最短路径顶点的下标
        for (int j = 0; j < _vexNum; ++j) {
            if (!s[j] && dist[j] < min) {
                u = j;
                min = dist[j];
            }
        }
        s[u] = true;//把u加入到s集合
        for (int k = 0; k < _vexNum; ++k) {	//更新
            w = _arcs[u][k].distance;
            if (!s[k] && w < INFINITY && dist[u] + w < dist[k]) {
                dist[k] = dist[u] + w;
                path[k] = u;
            }
        }
    }

}
int MyGraph::_minimun_closedge(closedge a)
{
    int min_weight =INFINITY ;
    int min = 0;
    for (int i = 0; i < _vexNum; ++i) {
        if (a[i].lowcost != 0 && a[i].lowcost < min_weight) {
            min_weight = a[i].lowcost;
            min = i;
        }
    }
    return min;
}
void MyGraph::_divide_trace(int v0, int v1, char* direction, int* sub_dist,int& count)
{
    const char* _trace = _arcs[v0][v1].track.data();

    for (int i = 0; i < _arcs[v0][v1].track.length(); ++i) {
        if (_trace[i] == 'e' || _trace[i] == 's' || _trace[i] == 'w' || _trace[i] == 'n')
            count++;
    }
    for (int j = 0; j < count; ++j) {
        direction[j] = _trace[j * 4];
        //cout << direction[j];
        sub_dist[j] = (_trace[j * 4 + 1] - '0') * 100 + (_trace[j * 4 + 2] - '0') * 10 + (_trace[j * 4 + 3] - '0');
        //cout << sub_dist[j] << endl;
    }
}
//解决中文乱码问题
string MyGraph::UTF8ToGB(const char* str)
{
    string result;
    WCHAR* strSrc;
    LPSTR szRes;

    //获得临时变量的大小
    int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    strSrc = new WCHAR[i + 1];
    MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

    //获得临时变量的大小
    i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
    szRes = new CHAR[i + 1];
    WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

    result = szRes;
    delete[]strSrc;
    delete[]szRes;

    return result;

}


