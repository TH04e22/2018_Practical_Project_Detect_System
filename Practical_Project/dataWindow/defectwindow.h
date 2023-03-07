#ifndef DEFECTWINDOW_H
#define DEFECTWINDOW_H

#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QRadioButton>
#include <QSqlRelationalTableModel>
#include <QtSql>
#include <QStackedWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QMap>
#include "product.h"

class DefectWindow: public QDialog
{
    Q_OBJECT
public:
    DefectWindow(QWidget* parent );
    void resizeEvent(QResizeEvent *event );
    void setGoodChecked();
    void setDefectChecked();
    void setDefect( QMap<QString,ErrorInfo> );
    void setTableWidget();
    void setTableWidget( QMap<QString,int> );
    void initTableWidget();
    void showTable();
    void hideTable();
    QMap<QString, int> getErrors();
    bool status();
public slots:
    void lunchSetting();
    void resetTableWidget();
    void update( int );
signals:
    void comfirmSignal();
private:
    QSqlQueryModel* temp;
    QHBoxLayout *radioButtonLayout;
    QHBoxLayout *pushButtonLayout;
    QVBoxLayout *vLayout;
    QRadioButton *goodRadioButton;
    QRadioButton *defectRadioButton;
    QButtonGroup *buttonGroup;
    QTableWidget *tableWidget;
    QPushButton *submitButton;
    QPushButton *cancelButton;
    QMap<QString,ErrorInfo> errorInfo;
    QMap<QString, int> errors;
};

#endif // DEFECTWINDOW_H
