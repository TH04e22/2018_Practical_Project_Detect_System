#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QMap>
#include <QCameraInfo>
#include <QStringListModel>

namespace Ui {
class settingDialog;
}

class settingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit settingDialog(QWidget *parent = nullptr);
    ~settingDialog();
    void update_camera();
    void setProductComboBox( QStringListModel* );
    QMap<QString,QString> getConfig();
    void setConfig( QMap<QString,QString> );
signals:
    void submitConfig( QMap<QString,QString> );
private slots:
    void on_cancelPushButton_clicked();

    void on_weightPushButton_clicked();

    void on_settingPushButton_clicked();

    void on_dataPushButton_clicked();

    void on_confirmPushButton_clicked();

private:
    Ui::settingDialog *ui;
};

#endif // DIALOG_H
