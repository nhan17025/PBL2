#ifndef ROOMDIALOG_H
#define ROOMDIALOG_H

#include "roomprice.h"
#include "room.h"
#include <QDialog>
#include <QVector>

namespace Ui {
class RoomDialog;
}

class RoomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RoomDialog(const QVector<Room>&, bool editMode = false, QWidget *parent = nullptr);
    ~RoomDialog();
    QString getRoomNumber() const;
    QString getRoomType() const;
    int getFloor() const;
    double getPrice() const;
    QString getState() const;
    void setRoomData(QString &roomnumber, QString &roomtype, int floor, double price, QString &state);
    void setRoomPrices(const QVector<RoomPrice>& p);
    void updateRoomPrice();

private:
    Ui::RoomDialog *ui;
    bool editMode;
    QVector<RoomPrice> prices;
    QVector<Room> rooms;
private slots:
    void on_btnAdd_clicked();
    void on_btnCancel_clicked();
    void on_cbRoomType_currentIndexChanged(int);
};

#endif // ROOMDIALOG_H
