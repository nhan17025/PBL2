#include "RoomPrice.h"

RoomPrice::RoomPrice(QString roomType, double price) {
    this->roomType = roomType;
    this->price = price;
}

QString RoomPrice::getRoomType() const {
    return roomType;
}

double RoomPrice::getPrice() const {
    return price;
}

void RoomPrice::setPrice(double price) {
    this->price = price;
}
void RoomPrice::setRoomType(QString rt){
    this->roomType = rt;
}