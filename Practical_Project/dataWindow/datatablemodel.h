#ifndef DATATABLEMODEL_H
#define DATATABLEMODEL_H
#include <QSqlTableModel>

class dataTableModel: public QSqlTableModel
{
public:
    dataTableModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
};

#endif // DATATABLEMODEL_H
