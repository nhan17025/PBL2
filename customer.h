#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <QMainWindow>

class customer
{
private:
    QString gname;
    QString cccd;
    QString sdt;
public:
    customer(const QString& name,const QString& cancuoc,const QString& dienthoai) : gname(name), cccd(cancuoc), sdt(dienthoai) {};
    QString getName() const;
    QString getCccd() const;
    QString getSdt() const;
    void setName(const QString&);
    void setSdt (const QString&);
};

#endif // CUSTOMER_H
