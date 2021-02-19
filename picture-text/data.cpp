#include "data.h"
#include <cmath>
#include <QMessageBox>
using std::max;
using std::min;

data1::data1(QWidget *parent)
{
    pat=parent;
}
data1::~data1()
{
    if(!imgSetted) return;//强迫症,程序必须正常返回.不能delete了个寂寞...
    for(int i=0;i<=width;i++)//二维动态数组
    {
        delete [] R[i];
        delete [] G[i];
        delete [] B[i];
        delete [] A[i];
        delete [] dt[i];
    }
    delete [] R;
    delete [] G;
    delete [] B;
    delete [] A;
    delete [] dt;
}
void data1::setImg(QString s)
{
    if(imgSetted)
    {
        for(int i=0;i<=width;i++)//二维动态数组
        {
            delete [] R[i];
            delete [] G[i];
            delete [] B[i];
            delete [] A[i];
            delete [] dt[i];
        }
        delete [] R;
        delete [] G;
        delete [] B;
        delete [] A;
        delete [] dt;
    }
    imgSetted=true;
    pic=s;
    img.load(s);
    width=img.width();
    height=img.height();
    R=new long long* [width+1];//+1保命,防止越界,下同
    G=new long long* [width+1];
    B=new long long* [width+1];
    A=new long long* [width+1];
    dt=new int* [width+1];
    for(int i=0;i<=width;i++)//建立动态的二维数组
    {
        R[i]=new long long[height+1];
        R[i][height]=background.red();//初始化那个用来保命的+1,下同
        G[i]=new long long[height+1];
        G[i][height]=background.green();
        B[i]=new long long[height+1];
        B[i][height]=background.blue();
        A[i]=new long long [height+1];
        A[i][height]=background.alpha();
        dt[i]=new int[height+1];
        dt[i][height]=0;
    }
    QColor color;
    for(int i=0;i<width;i++)//初始化前缀和,注意这里不能+1
    {
        for(int j=0;j<height;j++)
        {
            color=img.pixelColor(i,j);
            if(color.alpha())
            {
                R[i][j]=color.red();
                G[i][j]=color.green();
                B[i][j]=color.blue();
                A[i][j]=color.alpha();
                dt[i][j]=1;
            }
            else
            {
                R[i][j]=0;
                G[i][j]=0;
                B[i][j]=0;
                A[i][j]=0;
                dt[i][j]=0;
            }
        }
    }
    for(int i=1;i<=width;i++)//求二维前缀和
    {
        for(int j=1;j<=height;j++)
        {
            R[i][j]+=R[i-1][j]+R[i][j-1]-R[i-1][j-1];
            G[i][j]+=G[i-1][j]+G[i][j-1]-G[i-1][j-1];
            B[i][j]+=B[i-1][j]+B[i][j-1]-B[i-1][j-1];
            A[i][j]+=A[i-1][j]+A[i][j-1]-A[i-1][j-1];
            dt[i][j]+=dt[i-1][j]+dt[i][j-1]-dt[i-1][j-1];
        }
    }
}
QColor data1::average(int x1, int y1, int x2, int y2)
{
    if(x1>=width) x1=width-1;//保命,防止越界
    if(x1<0) x1=0;
    if(x2>=width) x1=width-1;
    if(x2<0) x2=0;
    if(y1>=height) y1=height-1;
    if(y1<0) y1=0;
    if(y2>=height) y2=height-1;
    if(y2<0) y2=0;
    long long sum;//区域内所有值的和
    long long dot=dt[x2][y2]+dt[x1][y1]-dt[x2][y1]-dt[x1][y2];
    QColor ansc=Qt::white;//平均颜色
    if(!dot)
    {
        ansc.setAlpha(0);
        return ansc;
    }
    sum=R[x2][y2]+R[x1][y1]-R[x2][y1]-R[x1][y2];
    ansc.setRed(sum/dot);
    sum=G[x2][y2]+G[x1][y1]-G[x2][y1]-G[x1][y2];
    ansc.setGreen(sum/dot);
    sum=B[x2][y2]+B[x1][y1]-B[x2][y1]-B[x1][y2];
    ansc.setBlue(sum/dot);
    sum=A[x2][y2]+A[x1][y1]-A[x2][y1]-A[x1][y2];
    ansc.setAlpha(sum/((y2-y1+1)*(x2-x1+1)));
    return ansc;
}
QString data1::getChar(int &pos)
{
    int size=text.size();
    while(pos>=size) pos-=size;//循环
    QString ans;
    ans.push_back(text[pos]);
    return ans;
}
double calculateRGBAValue(const double fTranslucent1, const double fTranslucent2, const double RGBVlue1, const double RGBVlue2)
{
return (RGBVlue1 * fTranslucent1 * (1.0 - fTranslucent2) + RGBVlue2 * fTranslucent2)
        / (fTranslucent1 + fTranslucent2 - fTranslucent1 * fTranslucent2);  //计算两个叠加后的值
}
QColor data1::merge(QColor p1, QColor p2)//合并颜色,p1为前景
{
    double a1,a2;
    a1=p1.alpha();a2=p2.alpha();
    a1/=255;a2/=255;
    if(a1+a2==0.0||a1+a2-a1*a2==0.0)
    {
        return QColor(0,0,0,0);
    }
    else if(a1==0.0) return p2;
    else if(a2==0.0) return p1;
    //算法来源:https://blog.csdn.net/u012377293/article/details/108606854
    //处理叠加的RGB和透明度
    //处理两种颜色叠加时透明度a-alpha值
#define backgroundColor p2
#define foregroundColor p1
    double fTranslucent1 = backgroundColor.alpha() / 255.0;
    double fTranslucent2 = foregroundColor.alpha() / 255.0;
    double fTranslucent = fTranslucent1 + fTranslucent2 - fTranslucent1 * fTranslucent2;
    //计算R-Red值
    double fRed1 = backgroundColor.red() / 255.0;
    double fRed2 = foregroundColor.red() / 255.0;
    double fRed = calculateRGBAValue(fTranslucent1, fTranslucent2, fRed1, fRed2);

    //计算G - Green值
    double fGreen1 = backgroundColor.green() / 255.0;
    double fGreen2 = foregroundColor.green() / 255.0;
    double fGreen = calculateRGBAValue(fTranslucent1, fTranslucent2, fGreen1, fGreen2);

    //计算B - Blue值
    double fBlue1 = backgroundColor.blue() / 255.0;
    double fBlue2 = foregroundColor.blue() / 255.0;
    double fBlue = calculateRGBAValue(fTranslucent1, fTranslucent2, fBlue1, fBlue2);
    return QColor(fRed * 255, fGreen * 255, fBlue * 255, fTranslucent * 255);
#undef foregroundColor
#undef backgroundColor
}

 void data1::create()
 {
     QImage tmp=img;
     tmp=tmp.scaled(int(width*scale),int(height*scale),Qt::KeepAspectRatio, Qt::SmoothTransformation);//缩放
     ans=QImage(tmp.width(),tmp.height(),QImage::Format_ARGB32);
     if(!mod) ans.fill(QColor(255,255,255,0));
     else ans.fill(background);
     QPainter painter(&ans);//设置画布
     painter.setPen(QPen(Qt::white));//初始化画笔
     painter.setFont(font);
     QFontMetrics fm = painter.fontMetrics();//用于计算每个字占的长宽
     int x=0,y=0,pos=0;//绘制的坐标和文字
     QString str=getChar(pos);
     int h=fm.ascent()+fm.descent();//文字高度,同一字体高度不变
     int w=fm.width(str);//文字长度,会改变,实时计算
     if(fixH+h<=0)
     {
         QMessageBox::critical(pat,"错误","纵向修正过大！");
         return;
     }
     int maxH=height*scale;//画布大小,下同
     int maxW=width*scale;
     bool stop=false;
     y=h+fixH;//注意(x,y)是绘制范围的左下角坐标!
     while(y+fixH<maxH)//保证不会出界
     {
         if(stop) break;//不循环(不会有人用吧?)
         x=0;//每次横向绘制都要初始化
         while(x+w<maxW)//要加上横向宽度才能保证不出界
         {
             if(mod)//纯净模式,设置画笔颜色
             {
                 QColor col=average(x/scale,(y-h)/scale,(x+w)/scale,y/scale);
                 painter.setPen(QPen(col));
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
     unsigned int *data = (unsigned int *)ans.bits();//直接访问,加快速度
     unsigned int *tmp_data = (unsigned int *)tmp.bits();
     int pixel=maxW*maxH;
     int red,green,blue,alpha;
     for(int i=0;i<pixel;i++)
     {
         red=qRed(data[i]);
         green=qGreen(data[i]);
         blue=qBlue(data[i]);
         alpha=qAlpha(data[i]);
         if(!mod)
         {
             red*=qRed(tmp_data[i]);red>>=8;//位运算提升速度,精度损失较小
             green*=qGreen(tmp_data[i]);green>>=8;
             blue*=qBlue(tmp_data[i]);blue>>=8;
             alpha*=qAlpha(tmp_data[i]);alpha>>=8;
             QColor ansc=merge(QColor(red,green,blue,alpha),background);
             red=ansc.red();blue=ansc.blue();green=ansc.green();alpha=ansc.alpha();
         }
         if(light>0)    //公式来源于:https://blog.csdn.net/maozefa/article/details/4493395
         {
             red*=(1/(1-light));
             green*=(1/(1-light));
             blue*=(1/(1-light));
             red=min(red,255);
             green=min(green,255);
             blue=min(blue,255);
         }
         else if(light<0)
         {
             red*=(1+light);
             green*=(1+light);
             blue*=(1+light);
         }
         data[i]=qRgba(red,green,blue,alpha);
     }
     result=QPixmap::fromImage(ans);
     tmp=QImage();//清空临时数据,释放内存
 }
