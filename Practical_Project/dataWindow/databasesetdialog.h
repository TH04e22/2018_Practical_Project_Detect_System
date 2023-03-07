#ifndef DATABASESETDIALOG_H
#define DATABASESETDIALOG_H
#include <QObject>
#include <QDebug>
#include <QEvent>
#include <QDialog>
#include <QLabel>
#include <QMap>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QTextCodec>
#include <QMessageBox>
class DatabaseSetDialog: public QDialog
{
    Q_OBJECT
public:
    explicit DatabaseSetDialog( QWidget* parent = nullptr );
    ~DatabaseSetDialog(){}
    bool event( QEvent* e );
    QMap<QString, QString> getConfig(); // 回傳當前設定
    void setConfig( QMap<QString,QString>& configRef ); // 獲取從xml讀取的設定
public slots:
    void setConfig(); // 按下設定按鈕時，保存設定
signals:
    void configSignal(); // 當按下設定按鈕時，傳送設定給數據畫面
private:
    QMap<QString, QString> *config;
    QLabel *hostLabel;
    QLabel *databaseNameLabel;
    QLabel *userLabel;
    QLabel *passwordLabel;
    QLineEdit *hostEdit;
    QLineEdit *databaseNameEdit;
    QLineEdit *userEdit;
    QLineEdit *passwordEdit;
    QPushButton *setButton;
    QPushButton *cancelButton;
    QGridLayout *gridLayout;
};

#endif // DATABASESETDIALOG_H
