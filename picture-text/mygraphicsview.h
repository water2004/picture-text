#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QObject>
#include <QWidget>
#include <QGraphicsView>
#include <QImage>
#include <QGraphicsScene>

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
    QGraphicsScene *scene;

signals:

public slots:
};

#endif // MYGRAPHICSVIEW_H
