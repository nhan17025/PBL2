#include "history.h"

History::History() {}

void History::setGuestName(const QString& name) { this->guestName = name; }
void History::setRoomNumber(const QString& room) { this->roomNumber = room; }
void History::setCheckIn(const QDateTime& time) { this->checkIn = time; }
void History::setCheckOut(const QDateTime& time) { this->checkOut = time; }
void History::setRoomCharge(double charge) { this->roomCharge = charge; }
void History::setServiceCharge(double charge) { this->serviceCharge = charge; }
void History::setDiscount(double disc) { this->discount = disc; }
void History::setTotalAmount(double total) { this->totalAmount = total; }
void History::setPaymentMethod(const QString& method) { this->paymentMethod = method; }
void History::setNote(const QString& n) { this->note = n; }
void History::setCccd(const QString& c) { this->cccd = c; }

QString History::getGuestName() const { return this->guestName; }
QString History::getRoomNumber() const { return this->roomNumber; }
QDateTime History::getCheckIn() const { return this->checkIn; }
QDateTime History::getCheckOut() const { return this->checkOut; }
double History::getRoomCharge() const { return this->roomCharge; }
double History::getServiceCharge() const { return this->serviceCharge; }
double History::getDiscount() const { return this->discount; }
double History::getTotalAmount() const { return this->totalAmount; }
QString History::getPaymentMethod() const { return this->paymentMethod; }
QString History::getNote() const { return this->note; }
QString History::getCccd() const { return this->cccd; }