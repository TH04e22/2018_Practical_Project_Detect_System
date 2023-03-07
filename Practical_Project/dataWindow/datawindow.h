#ifndef DATAWINDOW_H
#define DATAWINDOW_H

#include "database.h"
#include "databasesetdialog.h"
#include "defectwindow.h"
#include "imageviewer.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QDate>
#include <QDateTime>
#include <QTimer>
#include <QFile>
#include <QtXml>
#include <QStringListModel>
#include <QMap>
#include <QFileDialog>
#include <QChartView>
#include <QChart>
#include <QGridLayout>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include "scaledchart.h"
#include "scaledchartview.h"

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class DataWindow;
}

class DataWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DataWindow(QWidget *parent = nullptr, Database* db = nullptr);
    ~DataWindow();
    void resizeEvent(QResizeEvent *event );
public slots:
    void setStuffData( QModelIndex );
    void clearStuffData();
    void updateDefect();
    void setStatusButton();
    void setDayPieChart(QMap<QString,int>);
    void setDayBarChart(QMap<QString,QMap<QString,int>>);
private:
    void initDBConfig();
    void storeDBConfig();
    void connectDB();
    QString getStatusRadioButton();
    Ui::DataWindow *ui;
    QTimer *updateTimer;
    QMap<QString, QString> config;
    QMap<QString, int> errors;
    QStringListModel *product;
    DatabaseSetDialog* setDialog;
    Database *database;
    DefectWindow *defectWindow;
    ImageViewer *imageViewer;
    QSqlQuery sqlQuery;
    QModelIndex index;
    ScaledChartView *barChartView;
    QBarCategoryAxis *axisX;
    QValueAxis *axisY;
    QChartView *pieChartView;
    ScaledChart *barChart;
    QChart *pieChart;
    int mode = 0; // create mode
private slots:
    void setDBconfig();
    void updateCurrentTime();
    void on_actionabout_triggered();
    void on_actionset_triggered();
    void on_startDateTimeEdit_dateTimeChanged(const QDateTime &dateTime);
    void on_endDateTimeEdit_dateTimeChanged(const QDateTime &dateTime);
    void on_allRadioButton_clicked();
    void on_goodRadioButton_clicked();
    void on_defectRadioButton_clicked();
    void on_imageChoser_clicked();
    void on_statusPushButton_clicked();
    void on_imagePlusViewButton_clicked();
    void on_detectImagePlusViewButton_clicked();
    void on_detectedImageChoser_clicked();
    void on_actionadd_triggered();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pidComboBox_currentIndexChanged(const QString &arg1);
    void on_actionminus_triggered();
    void on_tabWidget_currentChanged(int index);
    void on_pushButton_3_clicked();
};

#endif // DATAWINDOW_H
