#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "data.h"
#include <QColorDialog>
#include <QFileDialog>
#include<QDragEnterEvent>
#include<QMimeData>
#include<QDropEvent>
#include<QUrl>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    data1 *dat;
    data1* getdat();
    QPixmap mmp;
    bool check();
    void setpic(QString url);

protected:
    void dragEnterEvent(QDragEnterEvent*event);//拖动进入事件
    void dropEvent(QDropEvent*event);

private slots:
    void on_choosePic_clicked();

    void on_OK_clicked();

    void on_pushButton_clicked();

    void on_pure_clicked();

    void on_detail_clicked();

    void on_repeat_clicked();

    void on_background_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
