#ifndef REVENUE_H
#define REVENUE_H

#include <QDate>
#include "history.h"

class Revenue
{
private:
    QDate date;
    int invoiceCount;
    double roomRevenue;
    double serviceRevenue;
    double totalRevenue;

public:
    Revenue();
    Revenue(const QDate& d);

    // Setters
    void setDate(const QDate& d);
    void setInvoiceCount(int count);
    void setRoomRevenue(double revenue);
    void setServiceRevenue(double revenue);
    void setTotalRevenue(double total);

    // Getters
    QDate getDate() const;
    int getInvoiceCount() const;
    double getRoomRevenue() const;
    double getServiceRevenue() const;
    double getTotalRevenue() const;
    void addInvoice(const History& invoice);
};

#endif // REVENUE_H