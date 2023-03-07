#include "settingdialog.h"
#include "ui_settingdialog.h"
#include <QFileDialog>

settingDialog::settingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settingDialog)
{
    ui->setupUi(this);

    //set lineEdit readOnly
    ui->weightLineEdit->setReadOnly(1);
    ui->weightLineEdit->setText("E:/dataset/Project/weights/yolov3-tiny_139900.weights");
    ui->settingLineEdit->setReadOnly(1);
    ui->settingLineEdit->setText("E:/dataset/Project/cfg/yolov3-tiny.cfg");
    ui->dataLineEdit->setReadOnly(1);
    ui->dataLineEdit->setText("E:/dataset/Project/cfg/project.data");
    ui->productComboBox->addItem("p00001");
    ui->productComboBox->setCurrentIndex(0);
}

settingDialog::~settingDialog()
{
    delete ui;
}

//close this dialog
void settingDialog::on_cancelPushButton_clicked()
{
    close();
}

void settingDialog::update_camera()
{
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    ui->cameraComboBox->clear();
    if( !cameras.isEmpty() ) {
        foreach( QCameraInfo info, cameras )
        {
            ui->cameraComboBox->addItem(info.description());
        }
        ui->cameraComboBox->setCurrentIndex(0);
        ui->confirmPushButton->setEnabled(true);
    } else {
        ui->cameraComboBox->addItem("No camera");
        ui->cameraComboBox->setCurrentIndex(0);
        ui->confirmPushButton->setEnabled(false);
    }
}

void settingDialog::setProductComboBox( QStringListModel* model )
{
    ui->productComboBox->setModel(model);
}


void settingDialog::on_weightPushButton_clicked()
{
    QString weightFile  = QFileDialog::getOpenFileName(this,
        tr("Open Weight File"), "/", tr("Weight Files (*.weights)"));
    if( !weightFile.isNull() )
        ui->weightLineEdit->setText(weightFile);
}

void settingDialog::on_settingPushButton_clicked()
{
    QString cfgFile  = QFileDialog::getOpenFileName(this,
        tr("Open Config File"), "/", tr("Config Files (*.cfg)"));
    if( !cfgFile.isNull())
        ui->settingLineEdit->setText(cfgFile);
}

void settingDialog::on_dataPushButton_clicked()
{
    QString dataFile  = QFileDialog::getOpenFileName(this,
        tr("Open Data File"), "/", tr("Data Files (*.data)"));
    if( !dataFile.isNull() )
        ui->dataLineEdit->setText(dataFile);
}

void settingDialog::on_confirmPushButton_clicked()
{
    emit submitConfig( getConfig() );
    close();
}

QMap<QString,QString> settingDialog::getConfig()
{
    QMap<QString,QString> config;
    config.insert("pid",ui->productComboBox->currentText());
    config.insert("weightFile",ui->weightLineEdit->text());
    config.insert("configFile",ui->settingLineEdit->text());
    config.insert("dataFile", ui->dataLineEdit->text());
    config.insert("cameraIndex",QString::number(ui->cameraComboBox->currentIndex()));
    return config;
}

void settingDialog::setConfig( QMap<QString,QString> config )
{
    ui->weightLineEdit->setText(config["weightFile"]);
    ui->settingLineEdit->setText(config["configFile"]);
    ui->dataLineEdit->setText(config["dataFile"]);
}
