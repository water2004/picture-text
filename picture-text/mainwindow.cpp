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
    //ui->massage->setText("正在读取并预处理图片...");
    QString s=QFileDialog::getOpenFileName(this,tr("选择图片"),dat->pic,tr("Images (*.png *.bmp *.jpg)"));
    if(s!="")//即打开图片被取消
    {
        dat->setImg(s);
        int with = ui->label->width();
        int height = ui->label->height();
        //QPixmap fitpixmap = dat->pix.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
        QPixmap fitpixmap = dat->pix.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
        ui->label->setPixmap(fitpixmap);
    }
}

void MainWindow::on_OK_clicked()//生成图片
{
    //ui->massage->setText("正在生成图片...");
    dat->scale=ui->scale->value();
    dat->light=double(ui->addLight->value())/100;//定义域!
    dat->text=ui->textEdit->toPlainText();
    if(!check()) return;//未填写完整
    dat->fixH=ui->fixH->value();
    dat->fixW=ui->fixW->value();
    QString save=QFileDialog::getSaveFileName(this,tr("Save Image"),"result.png",tr("Images (*.png *.bmp *.jpg)")); //选择路径
    if(save=="") return;
    dat->create();
    dat->ans.save(save);
    //ui->massage->setText("图片已保存为result.png");
}

void MainWindow::on_pushButton_clicked()//选择字体
{
    bool ok;
    //dat->font.setFamily("黑体");
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
    if(flag)
    {
        //ui->massage->setText("准备就绪");
    }
    return flag;
}

void MainWindow::on_background_clicked()
{
    QColor tmp=QColorDialog::getColor(dat->background);
    if(tmp!=QColor::Invalid)
    {
        dat->background=tmp;
    }
}
