#include "revenue.h"

Revenue::Revenue()
    : invoiceCount(0), roomRevenue(0.0), serviceRevenue(0.0), totalRevenue(0.0)
{
}

Revenue::Revenue(const QDate& d)
    : date(d), invoiceCount(0), roomRevenue(0.0), serviceRevenue(0.0), totalRevenue(0.0)
{
}

// === Setters ===
void Revenue::setDate(const QDate& d) { date = d; }
void Revenue::setInvoiceCount(int count) { invoiceCount = count; }
void Revenue::setRoomRevenue(double revenue) { roomRevenue = revenue; }
void Revenue::setServiceRevenue(double revenue) { serviceRevenue = revenue; }
void Revenue::setTotalRevenue(double total) { totalRevenue = total; }

// === Getters ===
QDate Revenue::getDate() const { return date; }
int Revenue::getInvoiceCount() const { return invoiceCount; }
double Revenue::getRoomRevenue() const { return roomRevenue; }
double Revenue::getServiceRevenue() const { return serviceRevenue; }
double Revenue::getTotalRevenue() const { return totalRevenue; }

// === Logic cộng dồn tự động ===
void Revenue::addInvoice(const History& invoice)
{
    invoiceCount++;
    roomRevenue += invoice.getRoomCharge();
    serviceRevenue += invoice.getServiceCharge();
    totalRevenue += invoice.getTotalAmount();
}