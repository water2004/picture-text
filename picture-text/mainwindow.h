#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "data.h"
#include <QColorDialog>
#include "preview.h"

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
