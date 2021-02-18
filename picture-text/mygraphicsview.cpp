#include "mygraphicsview.h"
#include <QWheelEvent>

MyGraphicsView::MyGraphicsView(QWidget *parent) : QGraphicsView(parent)
{

}

void MyGraphicsView::wheelEvent(QWheelEvent *event)
{
    if(event->delta()>0)
    {
        scale(1.25,1.25);
    }
    else
    {
        scale(0.8,0.8);
    }
}

void MyGraphicsView::init(QPixmap *pix)//初始化
{
    scene.clear();
    scene.addPixmap(*pix);//设置图片
    setScene(&scene);
    //初始化缩放比例
    resetTransform();
    resetMatrix();
    double ratio_h=height();
    double ratio_w=width();
    ratio_h/=pix->height();
    ratio_w/=pix->width();
    double ratio=ratio_h<ratio_w?ratio_h:ratio_w;
    scale(ratio,ratio);
}

void MyGraphicsView::refresh(QPixmap *pix)//刷新
{
    scene.clear();
    scene.addPixmap(*pix);
    setScene(&scene);
}
