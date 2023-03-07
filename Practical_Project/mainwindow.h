#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <dataWindow/datawindow.h>
#include <dataWindow/database.h>
#include <QThread>
#include <QTimer>
#include <QImage>
#include <QMap>
#include <QList>
#include "detect.h"
#include "settingdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QMap<QString,QString> config,QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void setTableWidget();
    void setAmount( int, int );
    void updateAmount();
    void updateData();
    void dataProcessing( QList<Component> components );
    void insertDatabase();
private slots:
    void on_testingPushButton_clicked();
    // void on_backToHomePagePushButton_clicked();
    //void on_settingPushButton_clicked();
    void on_startTestingPushButton_clicked();
    void on_pausePushButton_clicked();
    void updateCurrentTime();
    void on_checkingPushButton_clicked();
    void updateConfig( QMap<QString,QString> );
    /*void initNetworkConfig();
    void storeNetworkConfig();*/
private:
    Ui::MainWindow *ui;
    QTimer* timer;
    // settingDialog* sDialog;
    DataWindow* dataWindow;
    Database* database;
    QMap<QString,QString> config;
    Detect* detector;
    QThread* thread;
    QMap<QString, ComponentInfo> componentInfo;
    QMap<QString, QString> errorTable;
    QImage detectedImg;
    QImage Img;
    bool error;
    int productQuantity;
    int NGQuantity;
};

#endif // MAINWINDOW_H
