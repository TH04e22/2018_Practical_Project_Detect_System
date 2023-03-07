#include "database.h"
#include <QSpinBox>
#include <QDir>
#include <QTableWidgetItem>

Database::Database(QObject *parent) : QObject(parent)
{
    productDB = QSqlDatabase::addDatabase("QMYSQL");
    stuffDataModel = new dataTableModel( this, productDB );
}

bool Database::connectDB(QMap<QString, QString> &config)
{
    if( productDB.isOpen() )
        productDB.close();
    productDB.setHostName(config["host"]);
    productDB.setDatabaseName(config["database"]);
    productDB.setUserName(config["user"]);
    productDB.setPassword(config["password"]);

    if( productDB.open() ) {
        stuffDataModel->setTable("stuff");
        stuffDataModel->setHeaderData(0, Qt::Horizontal, "serial number" );
        stuffDataModel->setHeaderData(1, Qt::Horizontal, "product number" );
        stuffDataModel->setHeaderData(2, Qt::Horizontal, "product time" );
        stuffDataModel->setHeaderData(3, Qt::Horizontal, "image" );
        stuffDataModel->setHeaderData(4, Qt::Horizontal, "detected image" );
        stuffDataModel->setHeaderData(5, Qt::Horizontal, "status" );
        return true;
    } else
        return false;
}

bool Database::isConnect()
{
    if( productDB.isOpen() )
        return true;
    else
        return false;
}

QStringList Database::getProduct()
{
    QStringList stringList;
    if( productDB.isOpen() )
    {
        QSqlQuery sqlQuery;
        if(sqlQuery.exec("select * from product"))
        {
            while( sqlQuery.next() )
            {
                stringList.append(sqlQuery.value(0).toString());
            }
        }
    } else {
        stringList.append("None");
    }
    return stringList;
}

dataTableModel* Database::getProductDataModel()
{
    if( productDB.isOpen() )
    {
        stuffDataModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        stuffDataModel->setSort(2, Qt::DescendingOrder);
        stuffDataModel->select();
        return stuffDataModel;
    } else {
        return nullptr;
    }
}

void Database::setCondiction( QString productSid,QDateTime start, QDateTime end, QString status )
{
    QString format("yyyy-MM-dd hh:mm:ss");
    QString filter = QString("pid = '%1' and product_time between '%2' and '%3'")
            .arg(productSid).arg(start.toString(format)).arg(end.toString(format));
    if( status == "Good" ) {
        QString statusFilter = QString("and status = '%1'").arg("1");
        filter.append(statusFilter);
    } else if( status == "Defect" ) {
        QString statusFilter = QString("and status = '%1'").arg("0");
        filter.append(statusFilter);
    }
    stuffDataModel->setFilter(filter);

    if(!stuffDataModel->select()) {
        QMessageBox::critical(nullptr, "Unable to initialize Database",
                    "Error initializing database: " + stuffDataModel->lastError().text());
    }
}

QSqlRecord Database::getRecord( QModelIndex index )
{
    return stuffDataModel->record(index.row());
}

QString Database::getProductName( QString pid )
{
    QSqlQuery query("select name from product where pid = '"+ pid +"';",productDB);
    if(query.exec())
    {
        query.next();
        return query.value(0).toString();
    } else {
        return QString("None");
    }
}

QMap<QString,ErrorInfo> Database::getProductError( QString pid )
{
    QMap<QString, ErrorInfo> errorInfo;
    if( productDB.isOpen() )
    {
        QSqlQuery query("select error.eid as eid, error.name as name, error.describe as describle from error, product_have_component, component_have_defect where error.eid = component_have_defect.eid and product_have_component.cid = component_have_defect.cid and product_have_component.pid = '"+ pid +"';",productDB);
        if(query.exec()) {
            while(query.next())
            {
                ErrorInfo temp(query.value(0).toString(),query.value(1).toString(),query.value(2).toString());
                errorInfo.insert(query.value(0).toString(),temp);
            }
        }
    }
    return errorInfo;
}

QMap<QString,int> Database::getStuffError( QString sid )
{
    QMap<QString, int> errors;
    if( productDB.isOpen() )
    {
        QSqlQuery query("select eid, number from stuff_have_error where sid = '"+ sid +"';",productDB);
        if(query.exec()) {
            while(query.next())
            {
                errors.insert(query.value(0).toString(),query.value(1).toInt());
            }
        }
    }
    return errors;
}

bool Database::modifyRecord( int row,QSqlRecord& record, QMap<QString,int> errors,QString preSid )
{
    if(stuffDataModel->setRecord(row, record)) {
        QSqlQuery query("delete from stuff_have_error where sid = '"+ preSid +"';",productDB);
        query.exec();
        if(!insertErrors(record.value(0).toString(), errors)) {
            QSqlQuery query("delete from stuff_have_error where sid = '"+ record.value(0).toString() +"';",productDB);
            query.exec();
            stuffDataModel->revertAll();
            return false;
        }
        stuffDataModel->submitAll();
        stuffDataModel->select();
        return true;
    } else {
        stuffDataModel->revertAll();
        return false;
    }
}

bool Database::createRecord( QSqlRecord& record, QMap<QString,int> errors )
{
    if(stuffDataModel->insertRecord(-1, record)) {
        stuffDataModel->submitAll();
        if(!insertErrors(record.value(0).toString(), errors)) {
            QSqlQuery query("delete from stuff_have_error where sid = '"+ record.value(0).toString() +"';",productDB);
            query.exec();
            stuffDataModel->revertAll();
            return false;
        }
        stuffDataModel->select();
        return true;
    } else {
        stuffDataModel->revertAll();
        return false;
    }
}

bool Database::createRecord( QSqlRecord& record, QMap<QString, ComponentInfo> componentInfo )
{
    if(stuffDataModel->insertRecord(-1, record)) {
        stuffDataModel->submitAll();
        QMap<QString, ComponentInfo>::iterator it;
        for( it = componentInfo.begin(); it != componentInfo.end(); it++ )
        {
            if( !it->getErrors().isEmpty() ) {
                if(!insertErrors(record.value(0).toString(), it->getErrors())) {
                    QSqlQuery query("delete from stuff_have_error where sid = '"+ record.value(0).toString() +"';",productDB);
                    query.exec();
                    stuffDataModel->revertAll();
                    return false;
                }
            }
        }
        stuffDataModel->select();
        return true;
    } else {
        stuffDataModel->revertAll();
        return false;
    }
}

bool Database::deleteRecords( QModelIndexList list )
{
    QSqlQuery query(productDB);
    query.prepare("delete from stuff_have_error where sid = ?");
    QMap<QString,int>::iterator it;
    for( int i = 0; i < list.count(); i++ ) {
        QSqlRecord record = getRecord(list[i]);
        query.addBindValue(record.value("sid").toString());
        QFile image1("assembly_line/" + record.value("pid").toString() + "/"  + record.value("image").toString());
        image1.remove();
        QFile image2("assembly_line/" + record.value("pid").toString() + "/"  + record.value("detected_image").toString());
        image2.remove();
        if( !query.exec() ) {
            stuffDataModel->revertAll();
            return false;
        }

        if( !stuffDataModel->removeRow(list[i].row())) {
            stuffDataModel->revertAll();
            return false;
        }
    }
    stuffDataModel->submitAll();
    stuffDataModel->select();
    return true;
}

QMap<QString, ComponentInfo> Database::getComponentSetting( QString pid  )
{
    QSqlQuery query("select component.cid as cid, component.name as name, product_have_component.number as number from component, product_have_component where product_have_component.cid = component.cid and product_have_component.pid = '"+ pid +"';",productDB);
    QMap<QString, ComponentInfo> Info;
    if(query.exec())
    {
        while( query.next() )
        {
            ComponentInfo temp(query.value(0).toString(),query.value(1).toString(),query.value(2).toInt());
            if( temp.getCid() == "c00001" ) {
                temp.setLessError("e00001");
            } else if ( temp.getCid() == "c00002" ) {
                temp.setLessError("e00002");
            } else if ( temp.getCid() == "c00003" ) {
                temp.setLessError("e00003");
            } else if ( temp.getCid() == "c00004" ) {
                temp.setLessError("e00004");
            } else if ( temp.getCid() == "c00005" ) {
                temp.setLessError("e00005");
            } else if ( temp.getCid() == "c00006" ) {
                temp.setLessError("e00006");
            } else if ( temp.getCid() == "c00007" ) {
                temp.setLessError("e00007");
            }
            Info.insert( query.value(1).toString(), temp );
        }
        return Info;
    } else {
        return Info;
    }
}

QList<int> Database::getCounting( QDate currentDate, QString pid )
{
    int i = 0;
    QList<int> count;
    count << 0 << 0;
    QString str("select status,COUNT(status) from stuff where date(product_time) = curdate()  and pid = '" + pid + "' group by status");
    QSqlQuery query(str,productDB);
    if(query.exec())
    {
        while( query.next() )
        {
            count[query.value(0).toInt()] =  query.value(1).toInt();
        }
        return count;
    } else {
        return count;
    }
}

QString Database::getSid( QString pid, QDateTime currentTime )
{
    QSqlQuery query("select get_sid('" + pid + "' ,'" + currentTime.toString("yyyy-MM-dd HH:mm:ss.zzz") + "');",productDB);
    if(query.exec())
    {
        QString sid;
        query.next();
        sid =  query.value(0).toString();

        return sid;
    } else {
        QString sid("NULL");
        return sid;
    }
}

QMap<QString, QString> Database::getErrorTable( QString pid )
{
    QMap<QString, QString> errorTable;
    QSqlQuery query("select component_have_defect.eid as eid, component_have_defect.cid as cid from product_have_component, component_have_defect where product_have_component.cid = component_have_defect.cid and product_have_component.pid = '" + pid + "';", productDB );

    if(query.exec())
    {
        while( query.next() )
        {
            errorTable.insert(query.value(0).toString(), query.value(1).toString());
        }
    }

    return errorTable;
}

bool Database::insertErrors( QString sid, QMap<QString, int> errors )
{
    QSqlQuery query(productDB);
    query.prepare("INSERT INTO stuff_have_error (sid, eid, number) "
                      "VALUES (:sid, :eid, :number)");
    QMap<QString,int>::iterator it;
    for( it = errors.begin(); it!=errors.end();it++) {
        query.bindValue(0, sid );
        query.bindValue(1, it.key());
        query.bindValue(2, it.value());
        if(!query.exec()) {
            QSqlQuery query2("delete from stuff_have_error where sid = '"+ sid +"';",productDB);
            query2.exec();
            return false;
        }
    }
    return true;
}

QMap<QString,int> Database::getSingleDayData( QDate date, QString pid  )
{
    QMap<QString,int> data;
    QSqlQuery query("select status,count(status) from stuff where stuff.pid = '" + pid + "' and date(product_time) = date('" + date.toString("yyyy-MM-dd") + "')  group by status;",productDB);
    if( query.exec() ) {
        while( query.next() ) {
            data.insert(query.value(0).toString(), query.value(1).toInt());
        }
    }
    return data;
}

QMap<QString ,QMap<QString,int>> Database::getSingleDayEachHourData( QDate date, QString pid )
{
    QMap<QString,QMap<QString,int>> datas;
    QSqlQuery query("select hour(product_time), status, count(status) from stuff where pid = '" + pid + "' and date(product_time) = date('"+ date.toString("yyyy-MM-dd") +  "') group by status, hour(product_time);",productDB);
    if( query.exec() ) {
        while( query.next() ) {
            QMap<QString,int> data;
            if( datas.contains(query.value(0).toString()) ) {
                data = datas[query.value(0).toString()];
            }
            data.insert(query.value(1).toString(),query.value(2).toInt());
            datas.insert(query.value(0).toString(), data );
        }
    }

    QSqlQuery query2("select hour(stuff.product_time), stuff_have_error.eid, count(stuff_have_error.eid) from stuff, stuff_have_error where stuff.sid = stuff_have_error.sid and stuff.pid = '" + pid + "' and date(stuff.product_time) = date('"+ date.toString("yyyy-MM-dd") +  "') group by hour(stuff.product_time), stuff_have_error.eid ;",productDB);
    if( query2.exec() ) {
        while( query2.next() ) {
            QMap<QString,int> data;
            if( datas.contains(query2.value(0).toString()) ) {
                data = datas[query2.value(0).toString()];
            }
            data.insert(query2.value(1).toString(),query2.value(2).toInt());
            datas.insert(query2.value(0).toString(), data );
        }
    }

    return datas;
}
