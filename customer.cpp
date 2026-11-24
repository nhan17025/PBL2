#include "customer.h"

QString customer::getName() const {
    return this->gname;
}
QString customer::getCccd() const{
    return this->cccd;
}
QString customer::getSdt() const {
    return this->sdt;
}
void customer::setName(const QString& ten){
    this->gname = ten;
}
void customer::setSdt(const QString& dt){
    this->sdt = dt;
}