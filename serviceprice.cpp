#include "serviceprice.h"

serviceprice::serviceprice(QString namedv, double price) : namedv(namedv),price(price) {}
QString serviceprice::getnamedv() const{
    return this->namedv;
}
double serviceprice::getprice() const {
    return this->price;
}
void serviceprice::setprice(double &a){
    this->price = a;
}
void serviceprice::setnamedv(QString &a) {
    this->namedv = a;
}
