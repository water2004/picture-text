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

class data1 : public QObject
{
    Q_OBJECT
public:
    explicit data1(QObject *parent = nullptr);
    QString pic;//图片路径
    QPixmap pix;//原图
    QImage img;
    QFont font;//字体
    long long **R,**G,**B;//记录颜色值的前缀和数组
    /*  p1(x1,y1)  p2
     *
     *  p3  p4(x2,y2) */
    QColor average(int x1,int y1,int x2,int y2);//求原图平均颜色值
    int width,height;//原图宽和高
    void setImg(QString s);//设置图片
    ~data1();
    void create();//生成图片
    QImage ans;
    QString text;//要加入的文字
    QString getChar(int &pos);//取出text中位于pos的字符
    double scale;//缩放倍数
    bool mod=true;//true=pure false=detail
    int fixW,fixH;//横向修正大小和纵向修正大小
    bool repeat=true;//自动重复文字
    double light;//亮度补偿,-0.99~0.99
    QColor lighter(QColor c);//亮度补偿
    bool imgSetted=false;//没设置图片析构函数就delete了个寂寞,为了返回值的强迫症QAQ
signals:

public slots:
};

#endif // DATA_H
