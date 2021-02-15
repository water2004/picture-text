#include "mylabel.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <cmath>
using std::min;

myLabel::myLabel(QWidget *parent) : QLabel(parent)
{

}

void myLabel::mousePressEvent(QMouseEvent *event)//记录鼠标按下的位置
{
    pressed=true;
    x=event->x();
    y=event->y();
}

void myLabel::mouseReleaseEvent(QMouseEvent *event)
{
    pressed=false;
}

void myLabel::mouseMoveEvent(QMouseEvent *event)//移动鼠标相对移动的距离
{
    if(!setted) return;
    this->move(this->pos().x()+event->x()-x,this->pos().y()+event->y()-y);
}

void myLabel::wheelEvent(QWheelEvent *event)//鼠标滚轮事件
{
    if(!setted) return;
    if(event->delta()>0)
        addition*=1.25;
    else
    {
        addition/=1.25;
    }
    refresh(event->x(),event->y());
}

void myLabel::refresh(int mx,int my)//更新图片(主要用于缩放),mx,my是鼠标在图片坐标系中的坐标
{
    double wid=width();double hei=height();
    if(sourse->width()<sourse->height())//防止太小了找不着
    {
        if(addition*sourse->width()<=50) addition=50/double(sourse->width());
    }
    else
    {
        if(addition*sourse->height()<=50) addition=50/double(sourse->height());
    }
    while(sourse->width()*addition*sourse->height()*addition>134217728)//防止过度缩放爆内存
    {
        addition/=1.25;
    }
    //按比例缩放,同时有更新图片的作用
    myPic = sourse->scaled(sourse->width()*addition, sourse->height()*addition, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    resize(myPic.width(),myPic.height());
    double ratio=double(width())/wid;//缩放倍率
    wid=width();hei=height();
    double px=this->pos().x();double py=this->pos().y();//px,py为label左上角坐标
    if(ratio>1)
    {
        move(px-abs(double(mx)*(ratio-1)),py-abs(double(my)*(ratio-1)));//label移动该点距离的增量
    }
    else
    {
        move(px+abs(double(mx)*(ratio-1)),py+abs(double(my)*(ratio-1)));
    }
    setPixmap(myPic);//更新图片
    myPic=QPixmap();//用完就释放
}

void myLabel::init()//初始化
{
    setted=true;
    myPic = sourse->scaled(640, 360, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放,填满初始屏幕
    addition=double(min(myPic.width(),myPic.height()))/double(min(sourse->width(),sourse->height()));//初始化倍率
    move(0,0);//移动到初始位置
    resize(myPic.width(),myPic.height());
    setPixmap(myPic);
    myPic=QPixmap();//用完就释放
}
