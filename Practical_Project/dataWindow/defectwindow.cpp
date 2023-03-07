#include "defectwindow.h"
#include <QHeaderView>

DefectWindow::DefectWindow( QWidget* parent )
    :QDialog(parent)
{
    setWindowTitle("Select defect detail");
    setGeometry( parent->width()/2 + parent->x(),
                 parent->height()/2 - 250,
                 600, 450 );
    radioButtonLayout = new QHBoxLayout();
    pushButtonLayout = new QHBoxLayout();
    vLayout = new QVBoxLayout( this );

    goodRadioButton = new QRadioButton( "Good", this );
    defectRadioButton = new QRadioButton( "Defect", this );
    buttonGroup = new QButtonGroup( this );
    buttonGroup->addButton(goodRadioButton);
    buttonGroup->addButton(defectRadioButton);

    tableWidget = new QTableWidget( this );
    tableWidget->setColumnCount(5);
    tableWidget->verticalHeader()->hide();
    QStringList headers;
    headers  << "Checked" << "Num" << "Id" << "Name" << "Describe";
    tableWidget->setHorizontalHeaderLabels(headers);


    submitButton = new QPushButton( "Submit", this );
    cancelButton =  new QPushButton( "Cancel", this );

    radioButtonLayout->addWidget( goodRadioButton );
    radioButtonLayout->addWidget( defectRadioButton );
    pushButtonLayout->addWidget(submitButton);
    pushButtonLayout->addWidget(cancelButton);

    vLayout->addLayout(radioButtonLayout);
    vLayout->addWidget(tableWidget);
    vLayout->addLayout(pushButtonLayout);
    connect(buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(update(int)));
    connect(submitButton,SIGNAL(clicked()),this,SLOT(lunchSetting()));
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(resetTableWidget()));
}


void DefectWindow::setDefect( QMap<QString,ErrorInfo> errorInfo )
{
    this->errorInfo = errorInfo;
}

void DefectWindow::update(int id)
{
    if( id == -2 ) {
        hideTable();
    } else {
        showTable();
    }
}

void DefectWindow::setTableWidget() {
    QMap<QString,ErrorInfo>::iterator it;
    int count = 0;
    tableWidget->clearContents();
    tableWidget->setRowCount(errorInfo.count());
    for( it = errorInfo.begin(); it != errorInfo.end(); it++ ) {
        QCheckBox* checkbox = new QCheckBox();
        tableWidget->setCellWidget(count,0, checkbox);
        // it->setCheckButton(checkbox);
        QSpinBox* spinbox = new QSpinBox();
        tableWidget->setCellWidget(count,1, spinbox );
        // it->setSpinBox(spinbox);
        tableWidget->setItem(count,2,new QTableWidgetItem(it->getEid()));
        tableWidget->item(count,2)->setFlags(tableWidget->item(count,2)->flags() & ~Qt::ItemIsEditable);
        tableWidget->setItem(count,3,new QTableWidgetItem(it->getName()));
        tableWidget->item(count,3)->setFlags(tableWidget->item(count,3)->flags() & ~Qt::ItemIsEditable);
        tableWidget->setItem(count,4,new QTableWidgetItem(it->getDescribe()));
        tableWidget->item(count,4)->setFlags(tableWidget->item(count,4)->flags() & ~Qt::ItemIsEditable);
        count++;
    }
}

void DefectWindow::setTableWidget( QMap<QString,int> errors ) {
    this->errors = errors;
    for( int row = 0; row < tableWidget->rowCount(); row++ ) {
        QCheckBox* checkbox = dynamic_cast<QCheckBox*>(tableWidget->cellWidget(row,0));
        QSpinBox* spinbox = dynamic_cast<QSpinBox*>(tableWidget->cellWidget(row,1));
        if( errors.find(tableWidget->item(row,2)->text()) != errors.end() ) {
            tableWidget->cellWidget(row,1);
            int quantity = errors[tableWidget->item(row,2)->text()];
            checkbox->setChecked(true);
            spinbox->setValue(quantity);
        }
    }
}

void DefectWindow::initTableWidget() {
    for( int row = 0; row < tableWidget->rowCount(); row++ ) {
        QCheckBox* checkbox = dynamic_cast<QCheckBox*>(tableWidget->cellWidget(row,0));
        QSpinBox* spinbox = dynamic_cast<QSpinBox*>(tableWidget->cellWidget(row,1));
        checkbox->setChecked(false);
        spinbox->setValue(0);
    }
}


void DefectWindow::setGoodChecked() {
    hideTable();
    goodRadioButton->setChecked(true);
}

void DefectWindow::setDefectChecked() {
    showTable();
    defectRadioButton->setChecked(true);
}

void DefectWindow::resizeEvent(QResizeEvent *event)
{
    tableWidget->setColumnWidth(0, 10 * tableWidget->width()/100 );
    tableWidget->setColumnWidth(1, 10 * tableWidget->width()/100 );
    tableWidget->setColumnWidth(2, 20 * tableWidget->width()/100 );
    tableWidget->setColumnWidth(3, 20 * tableWidget->width()/100 );
    tableWidget->setColumnWidth(4, 40 * tableWidget->width()/100 );
    QDialog::resizeEvent(event);
}

void DefectWindow::showTable()
{
    for( int i = 0; i < tableWidget->rowCount(); i++ )
        tableWidget->showRow(i);

}

void DefectWindow::hideTable()
{
    for( int i = 0; i < tableWidget->rowCount(); i++ )
        tableWidget->hideRow(i);
}

QMap<QString, int> DefectWindow::getErrors()
{
    QMap<QString, int> errors;
    if( !status() ) {
        for( int row = 0; row < tableWidget->rowCount(); row++ ) {
            QCheckBox* checkbox = dynamic_cast<QCheckBox*>(tableWidget->cellWidget(row,0));
            QSpinBox* spinbox = dynamic_cast<QSpinBox*>(tableWidget->cellWidget(row,1));
            if( checkbox->isChecked() ) {
                errors.insert(tableWidget->item(row,2)->text(), spinbox->value());
            }
        }
    }
    return errors;
}

bool DefectWindow::status()
{
    if( goodRadioButton->isChecked() ) {
        return true;
    } else {
        return false;
    }
}

void DefectWindow::resetTableWidget() {
    initTableWidget();
    setTableWidget( errors );
}

void DefectWindow::lunchSetting() {
    emit comfirmSignal();
    close();
}



