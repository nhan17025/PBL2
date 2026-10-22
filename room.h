#ifndef ROOM_H
#define ROOM_H
#include <QMainWindow>
class Room
{
private:
    QString roomNumber;
    QString roomType;
    int floor;
    double price;
    QString state;
public:
    Room();
    void setroomNumber(const QString&);
    void setroomType(const QString&);
    void setfloor(const int&);
    void setprice(const double&);
    void setstate(const QString&);
    QString getroomNumber() const;
    QString getroomType() const;
    QString getroomstate() const;
    int getroomfloor() const;
    double getroomprice() const;
};

#endif // ROOM_H
