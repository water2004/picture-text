#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QWidget>
#include <QEvent>
#include <QPixmap>

class myLabel : public QLabel
{
    Q_OBJECT
public:
    explicit myLabel(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    int x=0,y=0;
    double addition=1.0;
    bool pressed=false;
    QPixmap *sourse=nullptr,myPic;
    void refresh(int mx,int my);
    void init();
    bool setted=false;

signals:

public slots:
};

#endif // MYLABEL_H
