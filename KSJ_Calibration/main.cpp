
#include "KSJDemoQT_Base.h"
#include <QtWidgets/QApplication>
#include <KSJPreviewWidget.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    KSJDemoQT_Base w;
    w.show();

    return a.exec();
}
