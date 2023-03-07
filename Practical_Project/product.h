#ifndef PRODUCT_H
#define PRODUCT_H
#include <QSet>
#include <QString>
#include <QCheckBox>
#include <QSpinBox>

class Component
{
public:
    Component( int classes, QString name, int top, int bottom, int left, int right ) {
        this->classes = classes;
        this->name = name;
        this->top = top;
        this->bottom = bottom;
        this->left = left;
        this->right = right;
    }

    ~Component() {}

    int getCenter_x() const
    {
        return ( right + left ) / 2;
    }

    int getCenter_y() const
    {
        return ( top + bottom ) / 2;
    }

    bool isBelongto( Component another ) const
    {
        if( left < another.getCenter_x() && another.getCenter_x() < right  && top < another.getCenter_y() && another.getCenter_y() < bottom )
            return true;
        else
            return false;
    }

    QString toString() const
    {
        return QString( name + " top:" + QString::number(top) + " left:" + QString::number(left) + " right:" + QString::number(right) + " bottom:" + QString::number(bottom));
    }

    QString getName() const
    {
        return name;
    }
private:
    int classes;
    QString name;
    int top, bottom, left, right;
};

class ComponentInfo
{
public:
    ComponentInfo() {
        this->cid = QString("Null");
        this->name = QString("Null");
        this->orderedQuantity = 0;
        producedQuantity = 0;
    }

    ComponentInfo( QString cid, QString name, int orderedQuantity ) {
        this->cid = cid;
        this->name = name;
        this->orderedQuantity = orderedQuantity;
        producedQuantity = 0;
    }

    void initInfo() {
        producedQuantity = 0;
        errors.clear();
    }

    void increasePQ() {
        producedQuantity++;
    }

    void addErrorStatus( QString status ) {
        if( errors.find(status) != errors.end() ){
            errors.insert(status,1);
        } else {
            int c = errors[status];
            errors[status] = ++c;
        }
    }
    QString getCid() const {
        return cid;
    }

    QString getName() const {
        return name;
    }

    int getOrdered() const {
        return orderedQuantity;
    }

    int getProduced() const {
        return producedQuantity;
    }

    QMap<QString,int> getErrors() const {
        return errors;
    }

    void insertError( QString name )
    {
        if( errors.contains(name)) {
            int temp = errors[name];
            temp++;
            errors[name] = temp;
        } else {
            errors.insert(name,1);
        }
    }

    void setLessError( QString e ) {
        lessError = e;
    }

    QString getLessError() const {
        return lessError;
    }

    void setredundantError( QString e ) {
        redundantError = e;
    }

    QString getredundantError() const {
        return redundantError;
    }
private:
    QString cid;
    QString name;
    QString lessError;
    QString redundantError;
    int orderedQuantity;
    int producedQuantity;
    QMap<QString,int> errors;
};

class ErrorInfo
{
public:
    ErrorInfo( QString eid, QString name, QString describe ) {
        this->eid = eid;
        this->name = name;
        this->describe = describe;
        checkBox = nullptr;
        spinbox = nullptr;
    }

    ~ErrorInfo() {
    }

    QString getEid() const {
        return eid;
    }

    QString getName() const {
        return name;
    }

    QString getDescribe() const {
        return describe;
    }

    void setCheckButton( QCheckBox* checkBox ) {
        this->checkBox = checkBox;
    }

    void setSpinBox( QSpinBox* spinbox ) {
        this->spinbox = spinbox;
        // spinbox->setMinimum(0);
    }

    void reset() {
        checkBox->setChecked(false);
        spinbox->setValue(0);
    }

    bool isChecked() const {
        if( checkBox == nullptr )
            return false;
        else if( checkBox->isChecked() )
            return true;
        else
            return false;
    }

    int errorQuantity() const {
        return spinbox->value();
    }
private:
    QString eid;
    QString name;
    QString describe;
    QCheckBox* checkBox;
    QSpinBox* spinbox;

};
#endif // PRODUCT_H
