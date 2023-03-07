#include "databasesetdialog.h"

DatabaseSetDialog::DatabaseSetDialog( QWidget* parent )
    :QDialog( parent )
{
    setGeometry(parent->x() + parent->width()/2,
                parent->y() + parent->height()/2,250,200);
    setWindowTitle(QStringLiteral("Database Setting"));
    hostLabel = new QLabel("Host: ",this);
    databaseNameLabel = new QLabel("Database: ",this);
    userLabel = new QLabel("User: ",this);
    passwordLabel = new QLabel("Password: ",this);
    hostEdit = new QLineEdit(this);
    databaseNameEdit = new QLineEdit(this);
    userEdit = new QLineEdit(this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);
    setButton = new QPushButton( "Set", this );
    gridLayout = new QGridLayout(this);

    gridLayout->addWidget(hostLabel,0,0);
    gridLayout->addWidget(hostEdit,0,1);
    gridLayout->addWidget(databaseNameLabel, 1, 0);
    gridLayout->addWidget(databaseNameEdit,1,1);
    gridLayout->addWidget(userLabel,2,0);
    gridLayout->addWidget(userEdit,2,1);
    gridLayout->addWidget(passwordLabel,3,0);
    gridLayout->addWidget(passwordEdit,3,1);
    gridLayout->addWidget(setButton,4,0,1,2);

    config = new QMap<QString, QString>();
    connect(setButton,SIGNAL(clicked()),this,SLOT(setConfig()));
}

bool DatabaseSetDialog::event(QEvent *e) {
    if (e->type() == QEvent::EnterWhatsThisMode) {
        QMessageBox::information(this,"About","Setting the database configure");
        return true;
    }
    return QWidget::event(e);
}


void DatabaseSetDialog::setConfig() {
    if( hostEdit->text() == "" ||
        databaseNameEdit->text() == "" ||
        userEdit->text() == ""||
            passwordEdit->text() == "" ) {
        QMessageBox::warning(this,"Warning","Field couldn't be empty!");
    } else {
        config->insert("host",hostEdit->text());
        config->insert("database",databaseNameEdit->text());
        config->insert("user",userEdit->text());
        config->insert("password",passwordEdit->text());
        emit configSignal();
        close();
    }
}


void DatabaseSetDialog::setConfig( QMap<QString,QString>& configRef ) {
    *config = configRef;
    hostEdit->setText(config->value("host"));
    databaseNameEdit->setText(config->value("database"));
    userEdit->setText(config->value("user"));
    passwordEdit->setText(config->value("password"));
}

QMap<QString, QString> DatabaseSetDialog::getConfig() {
    return (*config);
}

