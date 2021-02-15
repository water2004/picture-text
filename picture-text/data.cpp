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
    imgSetted=true;
    pic=s;
    pix.load(s);
    width=pix.width();
    height=pix.height();
    R=new long long* [width+1];//+1保命,防止越界,下同
    G=new long long* [width+1];
    B=new long long* [width+1];
    A=new long long* [width+1];
    dt=new long long* [width+1];
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
        dt[i]=new long long[height+1];
        dt[i][height]=0;
    }
    img=pix.toImage();
    QColor color;
    for(int i=0;i<width;i++)//初始化前缀和,注意这里不能+1
    {
        for(int j=0;j<height;j++)
        {
            color=img.pixelColor(i,j);
            R[i][j]=(long long)color.red();
            G[i][j]=(long long)color.green();
            B[i][j]=(long long)color.blue();
            A[i][j]=(long long)color.alpha();
            dt[i][j]=color.alpha()!=0;
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
    QColor ans=Qt::white;//平均颜色
    if(!dot)
    {
        ans.setAlpha(0);
        return ans;
    }
    sum=R[x2][y2]+R[x1][y1]-R[x2][y1]-R[x1][y2];
    ans.setRed(sum/dot);
    sum=G[x2][y2]+G[x1][y1]-G[x2][y1]-G[x1][y2];
    ans.setGreen(sum/dot);
    sum=B[x2][y2]+B[x1][y1]-B[x2][y1]-B[x1][y2];
    ans.setBlue(sum/dot);
    sum=A[x2][y2]+A[x1][y1]-A[x2][y1]-A[x1][y2];
    ans.setAlpha(sum/((y2-y1+1)*(x2-x1+1)));
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
    return QColor(newR,newG,newB,c.alpha());
}
float calculateRGBAValue(const float fTranslucent1, const float fTranslucent2, const float RGBVlue1, const float RGBVlue2)
{
return (RGBVlue1 * fTranslucent1 * (1.0 - fTranslucent2) + RGBVlue2 * fTranslucent2)
        / (fTranslucent1 + fTranslucent2 - fTranslucent1 * fTranslucent2);  //计算两个叠加后的值
}
QColor data1::merge(QColor p1, QColor p2)//合并颜色,p1为前景
{
    float a1,a2;
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
    float fTranslucent1 = backgroundColor.alpha() / 255.0;
    float fTranslucent2 = foregroundColor.alpha() / 255.0;
    float fTranslucent = fTranslucent1 + fTranslucent2 - fTranslucent1 * fTranslucent2;
    //计算R-Red值
    float fRed1 = backgroundColor.red() / 255.0;
    float fRed2 = foregroundColor.red() / 255.0;
    float fRed = calculateRGBAValue(fTranslucent1, fTranslucent2, fRed1, fRed2);

    //计算G - Green值
    float fGreen1 = backgroundColor.green() / 255.0;
    float fGreen2 = foregroundColor.green() / 255.0;
    float fGreen = calculateRGBAValue(fTranslucent1, fTranslucent2, fGreen1, fGreen2);

    //计算B - Blue值
    float fBlue1 = backgroundColor.blue() / 255.0;
    float fBlue2 = foregroundColor.blue() / 255.0;
    float fBlue = calculateRGBAValue(fTranslucent1, fTranslucent2, fBlue1, fBlue2);
    return QColor(fRed * 255, fGreen * 255, fBlue * 255, fTranslucent * 255);
#undef foregroundColor
#undef backgroundColor
}

 void data1::create()
 {
     QImage tmp=pix.toImage();//保存副本
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
     //result.save("debug.png");
     if(!mod)//细节模式,处理每个像素
     {
         for(int i=0;i<maxW;i++)
         {
             for(int j=0;j<maxH;j++)
             {
                 QColor col=tmp.pixelColor(i,j);
                 QColor co=ans.pixelColor(i,j);
                 QColor re;
                 re.setRed(co.red()*col.red()>>8);//按比例调整深浅(细节模式下画笔为白色)
                 re.setBlue(co.blue()*col.blue()>>8);//近似,加快计算,毕竟255和256也没差多少嘛
                 re.setGreen(co.green()*col.green()>>8);
                 re.setAlpha(col.alpha()*co.alpha()>>8);
                 re=merge(re,background);
                 re=lighter(re);//亮度补偿
                 ans.setPixelColor(i,j,re);
             }
         }
     }
 }
