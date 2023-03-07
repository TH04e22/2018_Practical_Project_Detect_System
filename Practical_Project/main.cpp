#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include "settingdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    settingDialog sDialog;
    sDialog.update_camera();
    sDialog.exec();
    MainWindow w(sDialog.getConfig());
    w.show();

    return a.exec();
}
