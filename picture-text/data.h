#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <QString>
#include <QPixmap>
#include <QImage>
#include <QFont>
#include <QRgb>
#include <QColor>
#include <QPainter>
#include <QPen>
#include <QMainWindow>
#include <QWidget>

class data1 : public QObject
{
    Q_OBJECT
public:
    explicit data1(QWidget *parent = nullptr);
    ~data1();
    /*  p1(x1,y1)  p2
     *
     *  p3  p4(x2,y2) */
    QColor average(int x1,int y1,int x2,int y2);//求原图平均颜色值
    QColor merge(QColor bak,QColor alp);
    QString getChar(int &pos);//取出text中位于pos的字符
    void setImg(QString s);//设置图片
    void create();//生成图片

    QWidget *pat;

    QString pic;//图片路径
    QString text;//要加入的文字

    QImage img;//存放原图
    QImage ans;//生成的图片
    QPixmap result;//用于显示

    QFont font;//字体
    long long **R,**G,**B,**A;//记录颜色值的前缀和数组
    int **dt;//记录非透明像素数
    int width,height;//原图宽和高
    int fixW,fixH;//横向修正大小和纵向修正大小
    double light;//亮度补偿,-0.99~0.99
    double scale;//缩放倍数
    bool mod=true;//true=pure false=detail
    bool repeat=true;//自动重复文字
    bool imgSetted=false;//没设置图片析构函数就delete了个寂寞,为了返回值的强迫症QAQ
    QColor background=Qt::black;

signals:

public slots:
};

#endif // DATA_H
