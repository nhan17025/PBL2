#ifndef HISTORY_H
#define HISTORY_H

#include <QString>
#include <QDateTime>

class History
{
private:
    QString guestName;
    QString cccd;
    QString roomNumber;
    QDateTime checkIn;
    QDateTime checkOut;
    double roomCharge;
    double serviceCharge;
    double discount;
    double totalAmount;
    QString paymentMethod;
    QString note;

public:
    History();
    void setGuestName(const QString& name);
    void setRoomNumber(const QString& room);
    void setCheckIn(const QDateTime& time);
    void setCheckOut(const QDateTime& time);
    void setRoomCharge(double charge);
    void setServiceCharge(double charge);
    void setDiscount(double disc);
    void setTotalAmount(double total);
    void setPaymentMethod(const QString& method);
    void setNote(const QString& n);
    void setCccd(const QString& c);

    QString getGuestName() const;
    QString getRoomNumber() const;
    QDateTime getCheckIn() const;
    QDateTime getCheckOut() const;
    double getRoomCharge() const;
    double getServiceCharge() const;
    double getDiscount() const;
    double getTotalAmount() const;
    QString getPaymentMethod() const;
    QString getNote() const;
    QString getCccd() const;
};

#endif // HISTORY_H