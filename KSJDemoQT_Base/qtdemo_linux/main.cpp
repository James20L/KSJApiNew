
#include "KSJDemoQT_Base.h"
#include <QtWidgets/QApplication>
#include <KSJPreviewWidget.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();



//    QWidget *test = new QWidget();

//    test->setGeometry(0,0,1000,1000);

//    test->show();



    KSJDemoQT_Base w;
    w.show();

//    CksjPreviewWidget  ksjpreviewthread;

//    ksjpreviewthread.show();

    return a.exec();
}
