#include "datatablemodel.h"
#include <QColor>
#include <QBrush>
#include <QDebug>
#include <QDateTime>

dataTableModel::dataTableModel(QObject *parent, QSqlDatabase db)
    :QSqlTableModel (parent, db)
{
}

QVariant dataTableModel::data(const QModelIndex& index, int role )const{
    if( role == Qt::BackgroundColorRole )
    {
        QModelIndex rowItem = QSqlTableModel::index( index.row(), 5, index.parent() );
        if( QSqlTableModel::data(rowItem, Qt::DisplayRole).toInt() )
        {
            return QColor(Qt::white);
        } else {
            return QColor(255,0,0,127);
        }
    }

    if( index.column() == 5 && role == Qt::DisplayRole )
    {
        if( QSqlTableModel::data(index, role).toInt() )
            return QVariant("Good");
        else
            return QVariant("Defect");
    }

    if( (index.column() == 3||index.column() == 4) && role == Qt::DisplayRole )
    {
        if( QSqlTableModel::data(index, role).toString().isEmpty() )
            return QVariant("NULL");
        else
            return QSqlTableModel::data(index, role);
    }

    if( index.column() == 2 && role == Qt::DisplayRole )
    {
        QDateTime dateTime = QSqlTableModel::data(index, role).toDateTime();
        return QVariant(dateTime.toString("yyyy/MM/dd hh:mm:ss.zzz"));
    }
    return QSqlTableModel::data(index, role);
}
