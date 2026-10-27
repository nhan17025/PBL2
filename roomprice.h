#ifndef ROOMPRICE_H
#define ROOMPRICE_H

#include <QString>

class RoomPrice {
private:
    QString roomType;
    double price;

public:
    RoomPrice(QString roomType, double price);

    QString getRoomType() const;
    double getPrice() const;
    void setRoomType(QString);
    void setPrice(double price);
};

#endif