#include "roomservices.h"

RoomServices::RoomServices() {}
double RoomServices::sum() {
    double tong = 0;
    for (const Service &sv : this->service) {
        tong += sv.getdg() * sv.getsl();
    }
    return tong;
}
void RoomServices::setroom(const QString& a) {
    this->room = a;
}
void RoomServices::addservice(const Service& a){
    int n=0;
    for (Service &sv : this->service){
        if (sv.getdv() == a.getdv()) {
            sv.setsl(a.getsl() + sv.getsl());
            n =1;
        }
    }
    if (!n) {
        service.append(a);
    }
}
QString RoomServices::getroom() const{
    return this->room;
}
QVector<Service> RoomServices::getservice() const{
    return this->service;
}
Service::Service(){}
double Service::getdg() const{
    return this->dg;
}
int Service::getsl() const{
    return this->sl;
}
QString Service::getdv() const{
    return this->dv;
}
void Service::setsl(const int& a){
    this->sl = a;
}
void Service::setdg(const double& a){
    this->dg = a;
}
void Service::setdv(const QString& a){
    this->dv = a;
}
void RoomServices::updateServicePrice(QString& serviceName, double newPrice) {
    for (int i = 0; i < service.size(); ++i) {
        if (service[i].getdv() == serviceName) {
            service[i].setdg(newPrice);
        }
    }
}