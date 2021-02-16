#include "preview.h"
#include "ui_preview.h"

preview::preview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::preview)
{
    ui->setupUi(this);
}

preview::~preview()
{
    delete ui;
}

void preview::setPic(QPixmap *pic)//设置图片
{
    ui->label->sourse=pic;
}

void preview::init()//初始化
{
    ui->label->init();
}

void preview::refresh()
{
    ui->label->refresh(0,0);//mx,my设为0,移动的距离也是0,只更新图片
}
