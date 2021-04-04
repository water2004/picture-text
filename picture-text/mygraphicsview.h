#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QObject>
#include <QWidget>
#include <QGraphicsView>
#include <QImage>
#include <QGraphicsScene>
#include <mainwindow.h>
#include<QDragEnterEvent>
#include<QMimeData>
#include<QDropEvent>
#include<QUrl>
#include <myscene.h>

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MyGraphicsView(QWidget *parent = nullptr);
    void wheelEvent(QWheelEvent *event);//缩放
    void init(QPixmap *pix);//初始化
    void refresh(QPixmap *pix);//刷新
    int Pwidth;
    ~MyGraphicsView();
    MyScene *scene;
    MainWindow *mainw;
    void set_main(MainWindow *w)
    {
        mainw=w;
    }

protected:
    void dropEvent(QDropEvent*event);

signals:

public slots:
};

#endif // MYGRAPHICSVIEW_H
