#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFileDialog>
#include <QSize>
#include <QPainter>
#include <QFontDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //初始化一些东西
    dat=new data1(this);
    ui->pure->setChecked(true);
    ui->repeat->setChecked(true);
    dat->mod=true;
    ui->textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->graphicsView->scene=new QGraphicsScene;
    ui->graphicsView->setScene(ui->graphicsView->scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}
data1* MainWindow::getdat()
{
    return dat;
}
void MainWindow::on_choosePic_clicked()//打开图片
{
    QString s=QFileDialog::getOpenFileName(this,tr("选择图片"),dat->pic,tr("Images (*.png *.bmp *.jpg)"));
    if(s!="")//成功打开图片
    {
        dat->setImg(s);
        dat->result=QPixmap::fromImage(dat->img);
        ui->graphicsView->init(&dat->result);
        dat->result=QPixmap();//释放内存
    }
}

void MainWindow::on_OK_clicked()//生成图片
{
    dat->scale=ui->scale->value();
    dat->light=double(ui->addLight->value())/100;//定义域!
    dat->text=ui->textEdit->toPlainText();
    if(!check()) return;//未填写完整
    dat->fixH=ui->fixH->value();
    dat->fixW=ui->fixW->value();
    QString save=QFileDialog::getSaveFileName(this,tr("Save Image"),"result.png",tr("Images (*.png *.bmp *.jpg)")); //选择路径
    if(save=="") return;
    dat->create();
    dat->result=QPixmap();
    dat->ans.save(save);
}

void MainWindow::on_pushButton_clicked()//选择字体
{
    bool ok;
    dat->font=QFontDialog::getFont(&ok,dat->font);
}

void MainWindow::on_pure_clicked()
{
    dat->mod=true;
}

void MainWindow::on_detail_clicked()
{
    dat->mod=false;
}

void MainWindow::on_repeat_clicked()
{
    dat->repeat=!dat->repeat;
}

bool MainWindow::check()//检查填写完整性
{
    bool flag=true;
    if(dat->text=="")
    {
        QMessageBox::critical(this,"错误","你还没有输入要写入图片的文字！");
        flag=false;
    }
    else if(dat->pic=="")
    {
        //ui->massage->setText("请选择图片");
        QMessageBox::critical(this,"错误","你还没有选择图片！");
        flag=false;
    }
    return flag;
}

void MainWindow::on_background_clicked()
{
    QColor tmp=QColorDialog::getColor(dat->background,this,tr("选择颜色"),QColorDialog::ShowAlphaChannel);
    if(tmp!=QColor::Invalid)
    {
        dat->background=tmp;
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    dat->scale=ui->scale->value();
    dat->light=double(ui->addLight->value())/100;//定义域!
    dat->text=ui->textEdit->toPlainText();
    if(!check()) return;//未填写完整
    dat->fixH=ui->fixH->value();
    dat->fixW=ui->fixW->value();
    if(!check()) return;
    dat->create();
    mmp=QPixmap::fromImage(dat->ans);
    ui->graphicsView->refresh(&dat->result);
    dat->result=QPixmap();//释放内存
}
