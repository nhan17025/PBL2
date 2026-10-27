#ifndef SERVICEPRICE_H
#define SERVICEPRICE_H
#include <QString>
class serviceprice
{
private:
    QString namedv;
    double price;
public:
    serviceprice(QString namedv,double price);
    QString getnamedv() const;
    double getprice() const;
    void setnamedv(QString &);
    void setprice(double &);
};

#endif // SERVICEPRICE_H
