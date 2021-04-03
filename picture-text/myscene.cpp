#include "myscene.h"

MyScene::MyScene()
{

}

void MyScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event){
//如果类型是jpg或者png才能接受拖动。
//这里的compare字符串比较函数，相等的时候返回0，所以要取反
   if(!event->mimeData()->urls()[0].fileName().right(3).compare("jpg")
           ||!event->mimeData()->urls()[0].fileName().right(3).compare("png"))
       event->accept();
    else
       event->ignore();//否则不接受鼠标事件
}

void MyScene::dropEvent(QDropEvent*event){
    const QMimeData*qm=event->mimeData();//获取MIMEData
    QString url=qm->urls()[0].toLocalFile();
    mainw->setpic(url);
}
