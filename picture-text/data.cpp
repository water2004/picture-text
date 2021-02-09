#include "data.h"
#include <cmath>
using std::max;
using std::min;

data1::data1(QObject *parent) : QObject(parent)
{

}
data1::~data1()
{
    if(!imgSetted) return;//强迫症,程序必须正常返回.不能delete了个寂寞...
    for(int i=0;i<=width;i++)//二维动态数组
    {
        delete [] R[i];
        delete [] G[i];
        delete [] B[i];
    }
    delete [] R;
    delete [] G;
    delete [] B;
}
void data1::setImg(QString s)
{
    imgSetted=true;
    pic=s;
    pix.load(s);
    width=pix.width();
    height=pix.height();
    R=new long long* [width+1];//+1保命,防止越界,下同
    G=new long long* [width+1];
    B=new long long* [width+1];
    for(int i=0;i<=width;i++)//建立动态的二维数组
    {
        R[i]=new long long[height+1];
        R[i][height]=0;//初始化那个用来保命的+1,下同
        G[i]=new long long[height+1];
        G[i][height]=0;
        B[i]=new long long[height+1];
        B[i][height]=0;
    }
    img=pix.toImage();
    QColor color;
    for(int i=0;i<width;i++)//初始化前缀和,注意这里不能+1
    {
        for(int j=0;j<height;j++)
        {
            color.setRgb(img.pixel(i,j));
            R[i][j]=(long long)color.red();
            G[i][j]=(long long)color.green();
            B[i][j]=(long long)color.blue();
        }
    }
    for(int i=1;i<=width;i++)//求二维前缀和
    {
        for(int j=1;j<=height;j++)
        {
            R[i][j]+=R[i-1][j]+R[i][j-1]-R[i-1][j-1];
            G[i][j]+=G[i-1][j]+G[i][j-1]-G[i-1][j-1];
            B[i][j]+=B[i-1][j]+B[i][j-1]-B[i-1][j-1];
        }
    }
}
QColor data1::average(int x1, int y1, int x2, int y2)
{
    if(x1>=width) x1=width-1;//保命,防止越界
    if(x2>=width) x1=width-1;
    if(y1>=height) y1=height-1;
    if(y2>=height) y2=height-1;
    long long sum;//区域内所有值的和
    long long dot=(y2-y1+1)*(x2-x1+1);//像素点的个数
    QColor ans;//平均颜色
    sum=R[x2][y2]+R[x1][y1]-R[x2][y1]-R[x1][y2];
    ans.setRed(sum/dot);
    sum=G[x2][y2]+G[x1][y1]-G[x2][y1]-G[x1][y2];
    ans.setGreen(sum/dot);
    sum=B[x2][y2]+B[x1][y1]-B[x2][y1]-B[x1][y2];
    ans.setBlue(sum/dot);
    return lighter(ans);//亮度补偿
}
QString data1::getChar(int &pos)
{
    int size=text.size();
    while(pos>=size) pos-=size;//循环
    QString ans;
    ans.push_back(text[pos]);
    return ans;
}
QColor data1::lighter(QColor c)
{
    //公式来源于:https://blog.csdn.net/maozefa/article/details/4493395
    if(light==0.0) return c;//不处理
    int R=c.red(),G=c.green(),B=c.blue();
    int newR,newG,newB;
    if(light>0)
    {
        newR=R*(1/(1-light));
        newG=G*(1/(1-light));
        newB=B*(1/(1-light));
        newR=min(newR,255);
        newG=min(newG,255);
        newB=min(newB,255);
    }
    else
    {
        newR=R*(1+light);
        newG=G*(1+light);
        newB=B*(1+light);
    }
    return QColor(newR,newG,newB);
}

 void data1::create()
 {
     QImage tmp=pix.toImage();//保存副本
     tmp=tmp.scaled(int(width*scale),int(height*scale),Qt::KeepAspectRatio, Qt::SmoothTransformation);//缩放
     QImage result=tmp;
     result.fill(Qt::black);
     QPainter painter(&result);//设置画布
     painter.setPen(QPen(QColor(255,255,255)));//初始化画笔
     painter.setFont(font);
     QFontMetrics fm = painter.fontMetrics();//用于计算每个字占的长宽
     int x=0,y=0,pos=0;//绘制的坐标和文字
     QString str=getChar(pos);
     int h=fm.ascent()+fm.descent();//文字高度,同一字体高度不变
     int w=fm.width(str);//文字长度,会改变,实时计算
     int maxH=height*scale;//画布大小,下同
     int maxW=width*scale;
     bool stop=false;
     y=h+fixH;//注意(x,y)是绘制范围的左下角坐标!
     while(y<maxH)//只需保证y<maxH即不会出界
     {
         if(stop) break;//不循环(不会有人用吧?)
         x=0;//每次横向绘制都要初始化
         while(x+w<maxW)//要加上横向宽度才能保证不出界
         {
             if(mod)//纯净模式,设置画笔颜色
             {
                 painter.setPen(QPen(average(x/scale,y/scale,(x+w)/scale,(y+h)/scale)));
             }
             painter.drawText(x,y,str);
             x+=w+fixW;//移位同时修正
             pos++;//绘制下一个字符
             if((!repeat)&&pos>=text.size())//不循环时已经结束了,跳出
             {
                 stop=true;
                 break;
             }
             str=getChar(pos);
             w=fm.width(str);//获取新的长度
         }
         y+=h+fixH;//移位同时修正
     }
     if(!mod)//细节模式,处理每个像素
     {
         for(int i=0;i<maxW;i++)
         {
             for(int j=0;j<maxH;j++)
             {
                 QColor col=tmp.pixel(i,j);
                 QColor co=result.pixel(i,j);
                 QColor re;
                 re.setRed(co.red()*col.red()/255);//按比例调整深浅(细节模式下画笔为白色)
                 re.setBlue(co.blue()*col.blue()/255);
                 re.setGreen(co.green()*col.green()/255);
                 re=lighter(re);//亮度补偿
                 result.setPixel(i,j,qRgb(re.red(),re.green(),re.blue()));
             }
         }
     }
     ans=result;//记录结果
 }
