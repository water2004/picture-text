#ifndef PREVIEW_H
#define PREVIEW_H

#include <QWidget>
#include <QEvent>
#include "data.h"
#include <QPixmap>
#include <QImage>

namespace Ui {
class preview;
}

class preview : public QWidget
{
    Q_OBJECT

public:
    explicit preview(QWidget *parent = nullptr);
    ~preview();
    data1 *dat;
    QPixmap *picture;
    void refresh();
    void setPic(QPixmap *pic);
    void init();

private:
    Ui::preview *ui;
};

#endif // PREVIEW_H
