#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QDateTime>
#include "room.h"
#include "guest.h"
#include "roomservices.h"
#include "roomprice.h"
#include "serviceprice.h"
#include "payment.h"
#include "history.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnRefreshRooms_clicked();
    void on_btnAddRoom_clicked();
    void on_btnDeleteRoom_clicked();
    void on_btnEditRoom_clicked();
    void on_btnSearchRoom_clicked();
    void on_btnConfirmBooking_clicked();
    void on_btnAddService_clicked();
    void on_cbServiceRoom_currentIndexChanged(int);
    void on_spinDiscount_valueChanged(int);
    void on_btnUpdateRoomType_clicked();
    void on_btnUpdateServiceCat_clicked();
    void on_cbServiceName_currentIndexChanged(int);
    void on_btnCheckOutAndPrint_clicked();
    void loadHistory();
    void saveHistory();
    void showHistory();
    void on_btnFilterHistory_clicked();
    void on_btnRefreshHistory_clicked();
private:
    Ui::MainWindow *ui;
    QVector<Room> rooms;
    QVector<guest> guests;
    QVector<RoomServices> roomservices;
    void saveRooms();
    void loadRooms();
    void loadAvaiRooms();
    void saveguest();
    void loadguest();
    void loadORooms();
    void saveRoomServices();
    void loadRoomServices();
    void showRoomServices();
    void loadBill();
    QVector<RoomPrice> roomPrices;
    void initRoomPrices();
    void loadRoomPrices();
    void saveRoomPrices();
    QVector<serviceprice> servicePrices;
    void initServicePrices();
    void loadServicePrices();
    void saveServicePrices();
    QVector<History> histories;
};

#endif // MAINWINDOW_H