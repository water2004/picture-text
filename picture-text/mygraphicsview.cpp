#include "mygraphicsview.h"
#include <QWheelEvent>

MyGraphicsView::MyGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    this->setAcceptDrops(true);
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
    Pwidth=pix->width();
    delete scene;
    scene=new MyScene;
    scene->addPixmap(*pix);//设置图片
    setScene(scene);
    //初始化缩放比例
    resetTransform();
    double ratio_h=height();
    double ratio_w=width();
    ratio_h/=pix->height();
    ratio_w/=pix->width();
    double ratio=ratio_h<ratio_w?ratio_h:ratio_w;
    scale(ratio,ratio);
}

void MyGraphicsView::refresh(QPixmap *pix)//刷新
{
    if(Pwidth>pix->width())
    {
        delete scene;
        scene=new MyScene;
        setScene(scene);
    }
    else
    {
        scene->clear();
    }
    Pwidth=pix->width();
    scene->addPixmap(*pix);
}

MyGraphicsView::~MyGraphicsView()
{
    delete scene;
}

void MyGraphicsView::dragEnterEvent(QDragEnterEvent*event){
//如果类型是jpg或者png才能接受拖动。
//这里的compare字符串比较函数，相等的时候返回0，所以要取反
   if(!event->mimeData()->urls()[0].fileName().right(3).compare("jpg")
           ||!event->mimeData()->urls()[0].fileName().right(3).compare("png"))
       event->acceptProposedAction();
    else
       event->ignore();//否则不接受鼠标事件
}

void MyGraphicsView::dropEvent(QDropEvent*event){
    const QMimeData*qm=event->mimeData();//获取MIMEData
    QString url=qm->urls()[0].toLocalFile();
    mainw->setpic(url);
}
