#ifndef GUEST_H
#define GUEST_H

#include <QMainWindow>
#include <QDateTime>

class guest
{
private:
    QString gname;
    QString cccd;
    QString sdt;
    QString room;
    QDateTime nroom;
    QDateTime troom;
    double tiencoc;
public:
    guest();
    guest(QString&, QString&, QString&, QString&, QDateTime&, QDateTime&, double&);
    void setgname(const QString&);
    void setcccd(const QString&);
    void setsdt(const QString&);
    void setroom(const QString&);
    void setnroom(const QDateTime&);
    void settroom(const QDateTime&);
    void settiencoc(const double&);

    QString getgname() const;
    QString getcccd() const;
    QString getsdt() const;
    QString getroom() const;
    QDateTime getnroom() const;
    QDateTime gettroom() const;
    double gettiencoc() const;
};

#endif // GUEST_H
