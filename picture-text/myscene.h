#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>
#include<QDragEnterEvent>
#include<QMimeData>
#include<QDropEvent>
#include<QUrl>
#include <mainwindow.h>
#include <QGraphicsSceneDragDropEvent>
class MyScene : public QGraphicsScene
{
    Q_OBJECT
public:
    MyScene();
    MainWindow *mainw;
    void set_main(MainWindow *w)
    {
        mainw=w;
    }

protected:
    //拖放代码参考
    //https://blog.csdn.net/fqx111/article/details/8858439
    //https://blog.csdn.net/a3631568/article/details/53819972
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);//拖动进入事件
    void dropEvent(QDropEvent*event);

signals:

public slots:
};

#endif // MYSCENE_H
