#include "mylabel.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <cmath>
using std::min;

myLabel::myLabel(QWidget *parent) : QLabel(parent)
{

}

void myLabel::mousePressEvent(QMouseEvent *event)
{
    pressed=true;
    x=event->x();
    y=event->y();
}

void myLabel::mouseReleaseEvent(QMouseEvent *event)
{
    pressed=false;
}

void myLabel::mouseMoveEvent(QMouseEvent *event)
{
    if(!setted) return;
    this->move(this->pos().x()+event->x()-x,this->pos().y()+event->y()-y);
    //setPixmap(myPic);
}

void myLabel::wheelEvent(QWheelEvent *event)
{
    if(!setted) return;
    if(event->delta()>0)
    addition*=1.25;
    else {
        addition/=1.25;
    }
    refresh(event->x(),event->y());
}

void myLabel::refresh(int mx,int my)
{
    double wid=width();double hei=height();
    if(sourse->width()<sourse->height())
    {
        if(addition*sourse->width()<=50) addition=50/double(sourse->width());
    }
    else
    {
        if(addition*sourse->height()<=50) addition=50/double(sourse->height());
    }
    myPic = sourse->scaled(sourse->width()*addition, sourse->height()*addition, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
    resize(myPic.width(),myPic.height());
    double ratio=double(width())/wid;
    wid=width();hei=height();
    double px=this->pos().x();double py=this->pos().y();
    if(ratio>1)
    {
        move(px-abs(double(mx)*(ratio-1)),py-abs(double(my)*(ratio-1)));
    }
    else
    {
        move(px+abs(double(mx)*(ratio-1)),py+abs(double(my)*(ratio-1)));
    }
    setPixmap(myPic);
}

void myLabel::init()
{
    setted=true;
    myPic = sourse->scaled(640, 360, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
    addition=double(min(myPic.width(),myPic.height()))/double(min(sourse->width(),sourse->height()));
    move(0,0);
    resize(myPic.width(),myPic.height());
    setPixmap(myPic);
}
