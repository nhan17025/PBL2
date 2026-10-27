#ifndef ROOMSERVICES_H
#define ROOMSERVICES_H
#include <QDialog>
#include <qvector.h>
class Service
{
private:
    int sl;
    double dg;
    QString dv;
public:
    int getsl() const;
    double getdg() const;
    QString getdv() const;
    void setsl(const int&);
    void setdg(const double&);
    void setdv(const QString&);
    Service();
};

class RoomServices
{
private:
    QString room;
    QVector<Service> service;
public:
    RoomServices();
    double sum();
    void setroom(const QString&);
    QString getroom() const;
    void addservice(const Service&);
    QVector<Service> getservice() const;
    void updateServicePrice(QString& serviceName, double newPrice);
};

#endif // ROOMSERVICES_H
