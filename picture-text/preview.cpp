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

void preview::setPic(QPixmap *pic)
{
    ui->label->sourse=pic;
}

void preview::init()
{
    ui->label->init();
}

void preview::refresh()
{
    ui->label->refresh(0,0);
}
