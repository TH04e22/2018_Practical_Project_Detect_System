#include "datawindow.h"
#include "ui_datawindow.h"
#include <QPixmap>
#include <QPieSeries>
#include <QPieSlice>
#include <QBarSet>
#include <QBarSeries>



DataWindow::DataWindow(QWidget *parent, Database *db) :
    QMainWindow(parent),
    ui(new Ui::DataWindow)
{
    ui->setupUi(this);
    ui->imageLineEdit->setReadOnly(true);
    ui->detectedImageLineEdit->setReadOnly(true);
    ui->productNameLineEdit->setReadOnly(true);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->statusBar->showMessage("Not connect to database!");
    ui->statusBar->setStyleSheet("background-color: rgb(255, 0, 0);");
    setDialog = new DatabaseSetDialog(this);
    database = db;
    product = new QStringListModel(this);

    imageViewer = new ImageViewer();

    axisX = new QBarCategoryAxis();
    axisY = new QValueAxis();
    axisY->setMin(0);
    barChart = new ScaledChart();
    pieChart = new QChart();
    barChartView = new ScaledChartView( barChart, this );
    barChart->setAxisX(axisX);
    barChart->setAxisY(axisY);

    pieChartView = new QChartView( pieChart, this );


    ui->produceGridLayout->addWidget(barChartView,0,0);
    ui->dataGridLayout->addWidget(pieChartView,0,0);
    ui->tabWidget->setCurrentIndex(0);


    initDBConfig();
    connectDB();


    ui->dateEdit->setDate(QDate::currentDate());
    setDayPieChart(database->getSingleDayData(QDate::currentDate(),ui->comboBox->currentText()));
    setDayBarChart(database->getSingleDayEachHourData(QDate::currentDate(),ui->comboBox->currentText()));

    defectWindow = new DefectWindow(this);
    defectWindow->setDefect(database->getProductError(ui->comboBox->currentText()));
    defectWindow->setTableWidget();
    defectWindow->setGoodChecked();

    ui->startDateTimeEdit->setDate(QDate::currentDate());
    ui->startDateTimeEdit->setTime(QTime(0,0,0));
    ui->endDateTimeEdit->setDate(QDate::currentDate());
    ui->endDateTimeEdit->setTime(QTime(23,59,59));
    updateTimer = new QTimer();
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->currentTimeEdit->setDateTime(QDateTime::currentDateTime());
    connect( setDialog, SIGNAL(configSignal()),this,SLOT(setDBconfig()));
    connect( updateTimer, SIGNAL(timeout()), this, SLOT(updateCurrentTime()));
    connect( ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateDefect()));
    connect( defectWindow, SIGNAL(comfirmSignal()),this, SLOT( setStatusButton() ) );
    updateTimer->start(1000);
    on_actionadd_triggered();
}

DataWindow::~DataWindow()
{
    storeDBConfig();
    delete imageViewer;
    delete ui;
}

void DataWindow::resizeEvent(QResizeEvent *event)
{
    ui->tableView->setColumnWidth(0, 25 * ui->tableView->width()/100);
    ui->tableView->setColumnWidth(1, 10 * ui->tableView->width()/100);
    ui->tableView->setColumnWidth(2, 20 * ui->tableView->width()/100);
    ui->tableView->setColumnWidth(3, 15 * ui->tableView->width()/100);
    ui->tableView->setColumnWidth(4, 15 * ui->tableView->width()/100);
    ui->tableView->setColumnWidth(5, 5 * ui->tableView->width()/100);
    QMainWindow::resizeEvent(event);
}

void DataWindow::updateCurrentTime() {
    ui->currentTimeEdit->setDateTime(QDateTime::currentDateTime());
}

void DataWindow::on_actionabout_triggered()
{
    QMessageBox::about(this,QStringLiteral("關於"),QStringLiteral("此頁面用於顯示數據與圖表"));
}


void DataWindow::on_actionset_triggered()
{
    setDialog->show();
}

void DataWindow::initDBConfig()
{
    QFile configFile("dbcfg.xml");
    if( !configFile.exists() ) {
        QMessageBox::critical(this,"Fatal Error","Not found configure file!<br>Please set configuration");
        setDialog->show();
    } else {
        if( !configFile.open(QIODevice::ReadOnly) ) {
            QMessageBox::critical(this,"Fatal Error","Can't open configure file!<br>Please set configuration");
            setDialog->show();
        } else {
            QDomDocument xml;
            xml.setContent(&configFile);
            QDomElement docElem = xml.documentElement();
            QDomNode n = docElem.firstChild();
            while(!n.isNull())
            {
               if (n.isElement())
               {
                  QDomElement e = n.toElement();
                  config.insert(e.tagName(),e.text());
                }
                  n = n.nextSibling();
            }
            setDialog->setConfig(config);
        }
        configFile.close();
    }
}

void DataWindow::storeDBConfig()
{
    QDomDocument doc;
    QDomProcessingInstruction instr = doc.createProcessingInstruction(
                        "xml", "version='1.0' encoding='UTF-8'");
    QDomElement dbcfg = doc.createElement("dbcfg");
    doc.appendChild(instr);
    doc.appendChild(dbcfg);
    QMap<QString,QString>::iterator i;
    for( i = config.begin(); i != config.end() ; i++ ) {
        QDomElement key = doc.createElement(i.key());
        QDomText value = doc.createTextNode(i.value());
        key.appendChild(value);
        dbcfg.appendChild(key);
    }
    QFile configFile("dbcfg.xml");
    if(!configFile.open(QIODevice::WriteOnly|QIODevice::Text)) {
        QMessageBox::critical(this,"Fatal Error","Can't write configure to file!");
        return;
    }
    QTextStream out(&configFile);
    doc.save(out,4);
    configFile.close();
}

void DataWindow::setDBconfig()
{
    config = setDialog->getConfig();
    connectDB();
}

void DataWindow::connectDB()
{
    if( database->connectDB( config ) ) {
        ui->statusBar->showMessage("MySQL Host: " + config["host"] + "    "
                + "Database: " + config["database"] +
                "    User: " + config["user"],0);
        ui->statusBar->setStyleSheet("background-color: rgb(0, 255, 0);");

        product->setStringList(database->getProduct());
        ui->comboBox->setModel(product);
        ui->comboBox->setCurrentIndex(0);


        ui->tableView->setModel(database->getProductDataModel());
        ui->tableView->setColumnWidth(0, 26 * ui->tableView->width()/100);
        ui->tableView->setColumnWidth(1, 10 * ui->tableView->width()/100);
        ui->tableView->setColumnWidth(2, 20 * ui->tableView->width()/100);
        ui->tableView->setColumnWidth(3, 30 * ui->tableView->width()/100);
        ui->tableView->setColumnWidth(4, 5 * ui->tableView->width()/100);

        connect( ui->tableView->selectionModel(), SIGNAL(currentRowChanged( const QModelIndex&, const QModelIndex& )),
                 this, SLOT(setStuffData(QModelIndex)) );
    } else {
        ui->tableView->setModel(database->getProductDataModel());
        ui->tableView->setColumnWidth(0, 26 * ui->tableView->width()/100);
        ui->tableView->setColumnWidth(1, 10 * ui->tableView->width()/100);
        ui->tableView->setColumnWidth(2, 20 * ui->tableView->width()/100);
        ui->tableView->setColumnWidth(3, 30 * ui->tableView->width()/100);
        ui->tableView->setColumnWidth(4, 5 * ui->tableView->width()/100);
        ui->statusBar->showMessage("Not connect to database!",0);
        ui->statusBar->setStyleSheet("background-color: rgb(255, 0, 0);");
        disconnect( ui->tableView->selectionModel(), SIGNAL(currentRowChanged( const QModelIndex&, const QModelIndex& )),
                 this, SLOT(setStuffData(QModelIndex)) );
    }

}

QString DataWindow::getStatusRadioButton()
{
    if( ui->goodRadioButton->isChecked() ) {
        return ui->goodRadioButton->text();
    } else if ( ui->defectRadioButton->isChecked() ) {
        return ui->defectRadioButton->text();
    } else {
        return ui->allRadioButton->text();
    }
}

void DataWindow::on_startDateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    if( ui->startDateTimeEdit->dateTime() > ui->endDateTimeEdit->dateTime() ) {
        ui->startDateTimeEdit->setDateTime( ui->endDateTimeEdit->dateTime() );
    }
    database->setCondiction( ui->comboBox->currentText(), ui->startDateTimeEdit->dateTime(), ui->endDateTimeEdit->dateTime(), getStatusRadioButton());
}

void DataWindow::on_endDateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    if( ui->endDateTimeEdit->dateTime() < ui->startDateTimeEdit->dateTime() ) {
        ui->endDateTimeEdit->setDateTime( ui->startDateTimeEdit->dateTime() );
    }
    database->setCondiction( ui->comboBox->currentText(), ui->startDateTimeEdit->dateTime(), ui->endDateTimeEdit->dateTime(), getStatusRadioButton());
}

void DataWindow::on_allRadioButton_clicked()
{
    database->setCondiction( ui->comboBox->currentText(), ui->startDateTimeEdit->dateTime(), ui->endDateTimeEdit->dateTime(), getStatusRadioButton());
}

void DataWindow::on_goodRadioButton_clicked()
{
    database->setCondiction( ui->comboBox->currentText(), ui->startDateTimeEdit->dateTime(), ui->endDateTimeEdit->dateTime(), getStatusRadioButton());
}

void DataWindow::on_defectRadioButton_clicked()
{
    database->setCondiction( ui->comboBox->currentText(), ui->startDateTimeEdit->dateTime(), ui->endDateTimeEdit->dateTime(), getStatusRadioButton());
}

void DataWindow::setStuffData(QModelIndex index)
{
    this->index = index;
    mode = 1; // modify mode
    defectWindow->initTableWidget();

    ui->groupBox_2->setTitle(QStringLiteral("修改數據"));
    ui->pushButton->setText(QStringLiteral("修改"));
    QSqlRecord record = database->getRecord(index);
    ui->imageLineEdit->setText(record.value("image").toString());
    ui->detectedImageLineEdit->setText(record.value("detected_image").toString());

    QPixmap image( "assembly_line/" + record.value("pid").toString() + "/" + record.value("image").toString() );
    if( image.isNull() ) {
        image.load("noimage.jpg");
    }
    /*image = image.scaled(200,ui->pictureView->height());
    ui->pictureView->setPixmap(image);*/

    ui->sidLineEdit->setText(record.value("sid").toString());
    ui->pidComboBox->setModel(product);
    ui->pidComboBox->setEditText(record.value("pid").toString());
    // ui->productNameLineEdit->setText(database->getProductName(record.value("pid").toString()));


    ui->stuffProductTime->setDateTime(record.value("product_time").toDateTime());

    if( record.value("status").toInt() )
    {
        ui->statusPushButton->setText(QStringLiteral("良好"));
        ui->statusPushButton->setStyleSheet("background-color: rgb(0, 255, 0); font-weight: bold;");
    } else {
        ui->statusPushButton->setText(QStringLiteral("缺陷"));
        ui->statusPushButton->setStyleSheet("background-color: rgb(255, 0, 0); font-weight: bold;");
    }
}

void DataWindow::on_imageChoser_clicked()
{
    QString filter("assembly_line/" + ui->pidComboBox->currentText());
    QString path(QFileDialog::getOpenFileName(this,QStringLiteral("選擇圖片"),filter,tr("Images (*.png *.bmp *.jpg)")));
    if( !path.isNull()) {
        QUrl url(path);
        ui->imageLineEdit->setText(url.fileName());
    }
}

void DataWindow::on_statusPushButton_clicked()
{
    if( database->isConnect() ) {
        if( ui->statusPushButton->text() == QStringLiteral("良好") ) {
           defectWindow->setGoodChecked();
        } else {
           defectWindow->setTableWidget(database->getStuffError(ui->sidLineEdit->text()));
           defectWindow->setDefectChecked();
        }
        defectWindow->show();
    } else
        QMessageBox::critical(this,"Fatal Error","Databases doesn't connect!");

}

void DataWindow::on_imagePlusViewButton_clicked()
{
    if(imageViewer->loadFile("assembly_line/" + ui->pidComboBox->currentText()+ "/" + ui->imageLineEdit->text()))
        imageViewer->show();
    else
        imageViewer->close();
}

void DataWindow::on_detectImagePlusViewButton_clicked()
{
    if(imageViewer->loadFile("assembly_line/" + ui->pidComboBox->currentText()+ "/" + ui->detectedImageLineEdit->text()))
        imageViewer->show();
    else
        imageViewer->close();
}

void DataWindow::on_detectedImageChoser_clicked()
{
    QString filter("assembly_line/" + ui->pidComboBox->currentText());
    QString path(QFileDialog::getOpenFileName(this,QStringLiteral("選擇圖片"),filter,tr("Images (*.png *.bmp *.jpg)")));
    if( !path.isNull()) {
        QUrl url(path);
        ui->detectedImageLineEdit->setText(url.fileName());
    }
}


void DataWindow::updateDefect()
{
    defectWindow->setDefect(database->getProductError(ui->comboBox->currentText()));
    defectWindow->setTableWidget();
}

void DataWindow::on_actionadd_triggered()
{
    mode = 0; // create mode
    clearStuffData();
    ui->groupBox_2->setTitle(QStringLiteral("新增數據"));
    ui->pushButton->setText(QStringLiteral("新增"));
    defectWindow->setGoodChecked();
}

void DataWindow::clearStuffData()
{
    ui->imageLineEdit->clear();
    ui->detectedImageLineEdit->clear();
    ui->sidLineEdit->clear();
    ui->pidComboBox->setModel(product);
    ui->stuffProductTime->setDateTime(QDateTime::currentDateTime());
    ui->statusPushButton->setText(QStringLiteral("良好"));
    ui->statusPushButton->setStyleSheet("background-color: rgb(0, 255, 0); font-weight: bold;");
}

void DataWindow::on_pushButton_clicked()
{
    if( mode ) { // modify mode
        QSqlRecord record = database->getRecord(index);
        QString presid =  record.value(0).toString();
        if( ui->sidLineEdit->text().isEmpty() ) {
            ui->statusBar->showMessage(QStringLiteral("修改失敗: sid 不可為空"));
            ui->statusBar->setStyleSheet("background-color: rgb(255, 0, 0);");
            return;
        }

        record.setValue("sid",ui->sidLineEdit->text());
        record.setValue("pid",ui->pidComboBox->currentText());
        record.setValue("product_time",ui->stuffProductTime->dateTime());
        if( ui->imageLineEdit->text().isEmpty() )
            record.setNull("image");
        else
            record.setValue("image",ui->imageLineEdit->text());

        if( ui->detectedImageLineEdit->text().isEmpty() )
            record.setNull("detected_image");
        else
            record.setValue("detected_image",ui->imageLineEdit->text());

        if( defectWindow->status() )
            record.setValue("status",1);
        else
            record.setValue("status",0);

        if(database->modifyRecord(index.row(),record,defectWindow->getErrors(),presid)){
            ui->statusBar->showMessage(QStringLiteral("修改成功"));
            ui->statusBar->setStyleSheet("background-color: rgb(0, 255, 0);");
        } else {
            ui->statusBar->showMessage(QStringLiteral("修改失敗"));
            ui->statusBar->setStyleSheet("background-color: rgb(255, 0, 0);");
        }
    } else {
        QSqlRecord record;
        if( ui->sidLineEdit->text().isEmpty() ) {
            ui->statusBar->showMessage(QStringLiteral("新增失敗: sid 不可為空"));
            ui->statusBar->setStyleSheet("background-color: rgb(255, 0, 0);");
            return;
        }

        record.append(QSqlField("sid",QVariant::Char));
        record.append(QSqlField("pid",QVariant::Char));
        record.append(QSqlField("product_time",QVariant::DateTime));
        record.append(QSqlField("image",QVariant::Char));
        record.append(QSqlField("detected_image",QVariant::Char));
        record.append(QSqlField("status",QVariant::Int));

        record.setValue("sid",ui->sidLineEdit->text());
        record.setValue("pid",ui->pidComboBox->currentText());
        record.setValue("product_time",ui->stuffProductTime->dateTime());

        if( ui->imageLineEdit->text().isEmpty() )
            record.setNull("image");
        else
            record.setValue("image",ui->imageLineEdit->text());

        if( ui->detectedImageLineEdit->text().isEmpty() )
            record.setNull("detected_image");
        else
            record.setValue("detected_image",ui->imageLineEdit->text());

        if( defectWindow->status() )
            record.setValue("status",1);
        else
            record.setValue("status",0);

        if(database->createRecord(record,defectWindow->getErrors())){
            ui->statusBar->showMessage(QStringLiteral("新增成功"));
            ui->statusBar->setStyleSheet("background-color: rgb(0, 255, 0);");
        } else {
            ui->statusBar->showMessage(QStringLiteral("新增失敗"));
            ui->statusBar->setStyleSheet("background-color: rgb(255, 0, 0);");
        }
    }
}

void DataWindow::on_pushButton_2_clicked()
{
    if( mode ) { // modify mode
        setStuffData(index);
    } else {
        clearStuffData();
    }
}

void DataWindow::on_pidComboBox_currentIndexChanged(const QString &arg1)
{
    ui->productNameLineEdit->setText(database->getProductName(arg1));
}

void DataWindow::setStatusButton() {
    if( defectWindow->status() )
    {
        ui->statusPushButton->setText(QStringLiteral("良好"));
        ui->statusPushButton->setStyleSheet("background-color: rgb(0, 255, 0); font-weight: bold;");
    } else {
        ui->statusPushButton->setText(QStringLiteral("缺陷"));
        ui->statusPushButton->setStyleSheet("background-color: rgb(255, 0, 0); font-weight: bold;");
    }
}

void DataWindow::on_actionminus_triggered()
{
    QMessageBox::StandardButton reply;
    //QString question("是否要刪除這" + QString::number(ui->tableView->selectionModel()->selectedIndexes().count()) + "筆記錄");
    reply = QMessageBox::question(this, QStringLiteral("刪除紀錄"),QStringLiteral("是否要刪除這幾筆紀錄?") ,
                                    QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        if(database->deleteRecords(ui->tableView->selectionModel()->selectedIndexes())) {
            ui->statusBar->showMessage(QStringLiteral("刪除成功"));
            ui->statusBar->setStyleSheet("background-color: rgb(0, 255, 0);");
        } else {
            ui->statusBar->showMessage(QStringLiteral("刪除失敗"));
            ui->statusBar->setStyleSheet("background-color: rgb(255, 0, 0);");
        }
    } else {
        return;
    }
}

void DataWindow::on_tabWidget_currentChanged(int index)
{
    if( index ) {
        ui->stackedWidget->setCurrentIndex(1);
        ui->stackedWidget_2->setCurrentIndex(1);
   } else {
        ui->stackedWidget->setCurrentIndex(0);
        ui->stackedWidget_2->setCurrentIndex(0);
    }
}

void DataWindow::setDayPieChart(QMap<QString,int> pieData)
{
    pieChart->removeAllSeries();
    pieChart->setTheme(QChart::ChartThemeLight);
    pieChart->setAnimationOptions(QChart::SeriesAnimations);
    QPieSeries* series = new QPieSeries();
    QList<QString> keys = pieData.keys();

    if( pieData.contains( "1" )) {
        QPieSlice* slice = series->append(QStringLiteral("Good"), pieData["1"]);
        slice->setColor(Qt::green);
        slice->setLabel("Good: " + QString::number(100*pieData["1"] /(pieData["1"]+ pieData["0"])) + "%");

    }

    if( pieData.contains( "0" )) {
        QPieSlice* slice = series->append(QStringLiteral("Defect"), pieData["0"]);
        slice->setColor(Qt::red);
        slice->setLabel("Defect: " + QString::number(100-100*pieData["1"] /(pieData["1"]+ pieData["0"])) + "%");
    }
    series->setHoleSize(0.35);
    pieChart->addSeries(series);
    pieChart->setTitle(ui->dateEdit->date().toString("yyyy/MM/dd") + QStringLiteral(" 良好數量: ") + QString::number(pieData["1"]) + QStringLiteral(" 缺陷數量: ") + QString::number(pieData["0"]));
}

void DataWindow::setDayBarChart(QMap<QString,QMap<QString,int>> barData)
{
    int large_number = 0;
    barChart->removeAllSeries();
    axisX->clear();
    QMap<QString,QBarSet*> barSets;
    QBarSeries* series = new QBarSeries();
    barSets.insert("Defect", new QBarSet("Defect"));
    barSets.insert("Good", new QBarSet("Good"));
    QStringList error = database->getProductError(ui->comboBox->currentText()).keys();
    for( QString& s: error ) {
        barSets.insert(s, new QBarSet(s));
    }
    QStringList category;
    for ( int i = 0; i < 24; i++ ) {
        category.append(QString::number(i));
        if( barData.contains(QString::number(i))) {
            if( barData[QString::number(i)].contains("1") )
                barSets["Good"]->append(barData[QString::number(i)]["1"]);
            else
                barSets["Good"]->append(0);

            if( barData[QString::number(i)].contains("0") )
                barSets["Defect"]->append(barData[QString::number(i)]["0"]);
            else
                barSets["Defect"]->append(0);

            for( QString& s: error ) {
                if(barData[QString::number(i)].contains(s))
                    barSets[s]->append(barData[QString::number(i)][s]);
                else {
                    barSets[s]->append(0);
                }
            }
        } else {
            barSets["Good"]->append(0);
            barSets["Defect"]->append(0);
            for( QString& s: error ) {
                 barSets[s]->append(0);
            }
        }

        if( barData[QString::number(i)]["1"] > large_number )
            large_number =  barData[QString::number(i)]["1"];

        if( barData[QString::number(i)]["0"] > large_number )
            large_number =  barData[QString::number(i)]["0"];
    }
    axisX->setCategories(category);
    axisY->setMax(large_number);
    barChart->setAnimationOptions(QChart::SeriesAnimations);
    barSets["Defect"]->setColor(Qt::red);
    barSets["Good"]->setColor(Qt::green);
    series->append(barSets["Good"]);
    series->append(barSets["Defect"]);

    for( QString& s: error ) {
         series->append(barSets[s]);
    }
    barChart->addSeries(series);
    //barChart->createDefaultAxes();
    // barChart->setAxisX(axisX);
    //series->attachAxis(axisY);
    series->attachAxis(axisX);
    barChart->legend()->setVisible(true);
    barChart->legend()->setAlignment(Qt::AlignBottom);
    barChartView->setRenderHint(QPainter::Antialiasing);
    barChart->setTitle(ui->dateEdit->date().toString("yyyy/MM/dd") );
}

void DataWindow::on_pushButton_3_clicked()
{
    switch(ui->comboBox_2->currentIndex()) {
        case 0:
            setDayPieChart(database->getSingleDayData(ui->dateEdit->date(), ui->comboBox->currentText()));
            setDayBarChart(database->getSingleDayEachHourData(ui->dateEdit->date(), ui->comboBox->currentText() ));
            break;
        default:
            break;
    }
}
