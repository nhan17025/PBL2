#include "room.h"

Room::Room() {}
void Room::setroomNumber(const QString& a){
    this->roomNumber = a;
}
void Room::setroomType(const QString& a){
    this->roomType = a;
}
void Room::setfloor(const int& a){
    this->floor = a;
}
void Room::setprice(const double& a){
    this->price = a;
}
void Room::setstate(const QString& a){
    this->state = a;
}
QString Room::getroomstate() const {
    return this->state;
}
QString Room::getroomNumber() const {
    return this->roomNumber;
}
QString Room::getroomType() const {
    return this->roomType;
}
int Room::getroomfloor() const {
    return this->floor;
}
double Room::getroomprice() const {
    return this->price;
}

