#include "guest.h"

guest::guest() {}
void guest::setgname(const QString& a) {
    this->gname = a;
}
void guest::setcccd(const QString& a) {
    this->cccd = a;
}
void guest::setsdt(const QString& a){
    this->sdt = a;
}
void guest::setroom(const QString& a){
    this->room = a;
}
void guest::setnroom(const QDateTime& a){
    this->nroom = a;
}
void guest::settroom(const QDateTime& a){
    this->troom = a;
}
void guest::settiencoc(const double& a) {
    this->tiencoc= a;
}
QString guest::getgname() const{
    return this->gname;
}
QString guest::getcccd() const {
    return this->cccd;
}
QString guest::getsdt() const{
    return this->sdt;
}
QString guest::getroom() const{
    return this->room;
}
QDateTime guest::getnroom() const{
    return this->nroom;
}
QDateTime guest::gettroom() const{
    return this->troom;
}
double guest::gettiencoc() const{
    return this->tiencoc;
}