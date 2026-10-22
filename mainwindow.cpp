#include "mainwindow.h"
#include "roomdialog.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this); // Thiết lập các thành phần giao diện từ file .ui
    loadRooms();
    loadAvaiRooms();
    loadguest();
    loadORooms();
    loadRoomServices();
    showRoomServices();
    loadBill();
}

MainWindow::~MainWindow() {
    delete ui;
}
void MainWindow::loadAvaiRooms() {
    ui->cbSelectRoom->clear();
    for (const Room &room : rooms) {
        if (room.getroomstate() == "Trống (Available)") {
            QString text = room.getroomNumber() + "-" + room.getroomType() + "-" + QString::number(room.getroomprice(),'f',0);
            ui->cbSelectRoom->addItem(text, room.getroomNumber());
         }
    }
}
void MainWindow::loadORooms() {
    ui->cbServiceRoom->clear();
    for (const Room &room : rooms) {
        if (room.getroomstate() == "Đang có khách (Occupied)") {
            QString text = room.getroomNumber() + "-" + room.getroomType() + "-" + QString::number(room.getroomprice(),'f',0);
            ui->cbServiceRoom->addItem(text, room.getroomNumber());
        }
    }
}
void MainWindow::saveRooms() {
    QJsonArray array;
    for (const Room &room : rooms){
        QJsonObject ob;
        ob["roomNumber"] = room.getroomNumber();
        ob["roomType"] = room.getroomType();
        ob["floor"] = room.getroomfloor();
        ob["price"] = room.getroomprice();
        ob["state"] = room.getroomstate();
        array.append(ob);
    }
    QJsonDocument doc(array);
    QFile file ("rooms.json");
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(doc.toJson());
        file.close();
    }
}
void MainWindow::saveguest() {
    QJsonArray array;
    for (const guest &khach : guests){
        QJsonObject ob;
        ob["cccd"] = khach.getcccd();
        ob["name"] = khach.getgname();
        ob["sdt"] = khach.getsdt();
        ob["room"] = khach.getroom();
        ob["NgayNhan"] = khach.getnroom().toString("dd/MM/yyyy HH:mm");
        ob["NgayTra"] = khach.gettroom().toString("dd/MM/yyyy HH:mm");
        ob["TienCoc"] = khach.gettiencoc();
        array.append(ob);
    }
    QJsonDocument doc(array);
    QFile file ("guests.json");
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(doc.toJson());
        file.close();
    }
}
void MainWindow::saveRoomServices()
{
    QJsonArray arr;

    for (const RoomServices &rs : roomservices)
    {
        QJsonObject obj;
        obj["room"] = rs.getroom();
        QJsonArray serviceArr;
        for (const Service &sv : rs.getservice())
        {
            QJsonObject serviceObj;
            serviceObj["Ten"] = sv.getdv();
            serviceObj["DonGia"] = sv.getdg();
            serviceObj["SoLuong"] = sv.getsl();
            serviceArr.append(serviceObj);
        }
        obj["DichVu"] = serviceArr;
        arr.append(obj);
    }
    QFile file("roomservices.json");
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(QJsonDocument(arr).toJson(QJsonDocument::Indented));
        file.close();
    }
}
void MainWindow::loadRoomServices()
{
    QFile file("roomservices.json");
    if (!file.open(QIODevice::ReadOnly))
        return;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    roomservices.clear();
    QJsonArray arr = doc.array();
    for (const QJsonValue &value : arr)
    {
        QJsonObject obj = value.toObject();
        RoomServices rsv;
        rsv.setroom(obj["room"].toString());
        QJsonArray serviceArr = obj["DichVu"].toArray();
        for (const QJsonValue &svValue : serviceArr)
        {
            QJsonObject serviceObj = svValue.toObject();
            Service sv;
            sv.setdv(serviceObj["Ten"].toString());
            sv.setdg(serviceObj["DonGia"].toDouble());
            sv.setsl(serviceObj["SoLuong"].toInt());
            rsv.addservice(sv);
        }
        roomservices.append(rsv);
    }
}
void MainWindow::loadguest() {
    QFile file ("guests.json");
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QByteArray data = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray())
    {
        return;
    }
    QJsonArray array = doc.array();
    guests.clear();
    for (const QJsonValue &value : array) {
        QJsonObject obj = value.toObject();
        guest khach;

        khach.setcccd (obj["cccd"].toString());
        khach.setgname( obj["name"].toString());
        khach.setsdt( obj["sdt"].toString());
        khach.settiencoc( obj["TienCoc"].toDouble());
        khach.setroom(obj["room"].toString());
        khach.setnroom(QDateTime::fromString(obj["NgayNhan"].toString(),"dd/MM/yyyy HH:mm"));
        khach.settroom(QDateTime::fromString(obj["NgayTra"].toString(),"dd/MM/yyyy HH:mm"));
        guests.append(khach);
    }
    ui->tableBookings->setRowCount(0);
    for (const guest &khach : guests) {
        int row = ui->tableBookings->rowCount();
        ui->tableBookings->insertRow(row);

        QString cccd = khach.getcccd();
        QString gname = khach.getgname();
        QString sdt = khach.getsdt();
        double tiencoc = khach.gettiencoc();
        QString room = khach.getroom();
        QDateTime nroom = khach.getnroom();
        QDateTime troom = khach.gettroom();

        ui->tableBookings->setItem(row,0,new QTableWidgetItem(gname));
        ui->tableBookings->setItem(row,1,new QTableWidgetItem(cccd));
        ui->tableBookings->setItem(row,2,new QTableWidgetItem(sdt));
        ui->tableBookings->setItem(row,3,new QTableWidgetItem(room));
        ui->tableBookings->setItem(row,4,new QTableWidgetItem(nroom.toString("dd/MM/yyyy HH:mm")));
        ui->tableBookings->setItem(row,5,new QTableWidgetItem(troom.toString("dd/MM/yyyy HH:mm")));
        ui->tableBookings->setItem(row,6,new QTableWidgetItem(QString::number(tiencoc)));
    }

}
void MainWindow::loadRooms(){
    QFile file ("rooms.json");
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QByteArray data = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray())
    {
        return;
    }
    QJsonArray array = doc.array();
    rooms.clear();
    for (const QJsonValue &value : array) {
        QJsonObject obj = value.toObject();
        Room room;

        room.setroomNumber(obj["roomNumber"].toString());
        room.setroomType(obj["roomType"].toString());
        room.setfloor(obj["floor"].toInt());
        room.setprice(obj["price"].toDouble());
        room.setstate(obj["state"].toString());

        rooms.append(room);
    }
    ui->tableRooms->setRowCount(0);
    for (const Room &room : rooms){
        int row = ui->tableRooms->rowCount();
        ui->tableRooms->insertRow(row);

        QString roomnumber = room.getroomNumber();
        QString roomtype = room.getroomType();
        int floor = room.getroomfloor();
        double price = room.getroomprice();
        QString state = room.getroomstate();

        ui->tableRooms->setItem(row, 0, new QTableWidgetItem(roomnumber));
        ui->tableRooms->setItem(row, 1, new QTableWidgetItem(roomtype));
        ui->tableRooms->setItem(row, 2, new QTableWidgetItem(QString::number(floor)));
        ui->tableRooms->setItem(row, 3, new QTableWidgetItem(QString::number(price,'f',0)));
        ui->tableRooms->setItem(row, 4, new QTableWidgetItem(state));

    }

}
void MainWindow::on_btnAddRoom_clicked()
{
    RoomDialog dialog(false, this);
    if (dialog.exec() == QDialog::Accepted) {

        QString roomnumber = dialog.getRoomNumber();
        QString roomtype = dialog.getRoomType();
        int floor = dialog.getFloor();
        double price = dialog.getPrice();
        QString state = dialog.getState();

        Room room;

        room.setroomNumber(roomnumber);
        room.setroomType(roomtype);
        room.setfloor(floor);
        room.setprice(price);
        room.setstate(state);


        rooms.append(room);
        saveRooms();
        loadAvaiRooms();
        loadORooms();
        int row = ui->tableRooms->rowCount();
        ui->tableRooms->insertRow(row);

        ui->tableRooms->setItem(row, 0, new QTableWidgetItem(roomnumber));
        ui->tableRooms->setItem(row, 1, new QTableWidgetItem(roomtype));
        ui->tableRooms->setItem(row, 2, new QTableWidgetItem(QString::number(floor)));
        ui->tableRooms->setItem(row, 3, new QTableWidgetItem(QString::number(price, 'f', 0)));
        ui->tableRooms->setItem(row, 4, new QTableWidgetItem(state));
    }
}
void MainWindow::on_btnDeleteRoom_clicked(){
    for (int i =ui->tableRooms->rowCount()-1; i >= 0;i--){
        if (ui->tableRooms->selectionModel()->isRowSelected(i, QModelIndex())){
            ui->tableRooms->removeRow(i);
            rooms.removeAt(i);
        }
    }
    saveRooms();
    loadAvaiRooms();
    loadORooms();
}
void MainWindow::on_btnEditRoom_clicked(){
    RoomDialog dialog(true, this);
    if (ui->tableRooms->rowCount() > 0) {
        int row = ui->tableRooms->currentRow();
        QString rnm = ui->tableRooms->item(row,0)->text();
        QString rt = ui->tableRooms->item(row,1)->text();
        int flo = ui->tableRooms->item(row,2)->text().toInt();
        double pr = ui->tableRooms->item(row,3)->text().toDouble();
        QString st = ui->tableRooms->item(row,4)->text();
        dialog.setRoomData(rnm,rt,flo,pr,st);
        if (dialog.exec() == QDialog::Accepted) {
            QString roomnumber = dialog.getRoomNumber();
            QString roomtype = dialog.getRoomType();
            int floor = dialog.getFloor();
            double price = dialog.getPrice();
            QString state = dialog.getState();

            rooms[row].setroomNumber(roomnumber);
            rooms[row].setroomType(roomtype);
            rooms[row].setfloor(floor);
            rooms[row].setprice(price);
            rooms[row].setstate(state);

            ui->tableRooms->setItem(row, 0, new QTableWidgetItem(roomnumber));
            ui->tableRooms->setItem(row, 1, new QTableWidgetItem(roomtype));
            ui->tableRooms->setItem(row, 2, new QTableWidgetItem(QString::number(floor)));
            ui->tableRooms->setItem(row, 3, new QTableWidgetItem(QString::number(price, 'f', 0)));
            ui->tableRooms->setItem(row, 4, new QTableWidgetItem(state));
            saveRooms();
            loadAvaiRooms();
            loadORooms();
        }
    }
}
void MainWindow::on_btnSearchRoom_clicked(){
    QString search = ui->txtSearchRoom->text();
    QString sta = ui->cbStatusFilter->currentText();
    QString type = ui->cbTypeFilter->currentText();
    for (int i = ui->tableRooms->rowCount()-1; i>=0;i--){
        if ((ui->tableRooms->item(i,0)->text() == search || search == "") && (ui->tableRooms->item(i,1)->text() == type || type == "-- Tất cả loại phòng --") && (ui->tableRooms->item(i,4)->text() == sta || sta =="-- Tất cả trạng thái --")) {
            ui->tableRooms->setRowHidden(i,false);
        } else{
            ui->tableRooms->setRowHidden(i,true);
        }
    }
}
void MainWindow::on_btnRefreshRooms_clicked() {
    for (int i = ui->tableRooms->rowCount()-1; i>=0;i--){
        ui->tableRooms->setRowHidden(i,false);
    }
    ui->txtSearchRoom->setText("");
}
void MainWindow::on_btnConfirmBooking_clicked() {
    guest khach;

    QString gname = ui->txtGuestName->text();
    QString cccd = ui->txtGuestID->text();
    QString sdt = ui->txtGuestPhone->text();
    QString room = ui->cbSelectRoom->currentText();
    QDateTime  nroom = ui->dtCheckIn->dateTime();
    QDateTime troom = ui->dtCheckOut->dateTime();
    double tiencoc = ui->spinDeposit->value();
    QString roomdata = ui->cbSelectRoom->currentData().toString();
    khach.setgname(gname);
    khach.setcccd(cccd);
    khach.setsdt(sdt);
    khach.setroom(roomdata);
    khach.setnroom(nroom);
    khach.settroom(troom);
    khach.settiencoc(tiencoc);
    guests.append(khach);

    int row=ui->tableBookings->rowCount();
    ui->tableBookings->insertRow(row);

    ui->tableBookings->setItem(row,0,new QTableWidgetItem(gname));
    ui->tableBookings->setItem(row,1,new QTableWidgetItem(cccd));
    ui->tableBookings->setItem(row,2,new QTableWidgetItem(sdt));
    ui->tableBookings->setItem(row,3,new QTableWidgetItem(room));
    ui->tableBookings->setItem(row,4,new QTableWidgetItem(nroom.toString("dd/MM/yyyy HH:mm")));
    ui->tableBookings->setItem(row,5,new QTableWidgetItem(troom.toString("dd/MM/yyyy HH:mm")));
    ui->tableBookings->setItem(row,6,new QTableWidgetItem(QString::number(tiencoc)));
    for (Room &room : rooms) {
        if (room.getroomNumber() == roomdata) {
            room.setstate("Đang có khách (Occupied)");
        }
    }
    saveRooms();
    loadRooms();
    loadAvaiRooms();
    saveguest();
    loadORooms();
}
void MainWindow::on_btnAddService_clicked(){
    RoomServices rsv;
    Service sv;
    rsv.setroom(ui->cbServiceRoom->currentData().toString());
    sv.setsl(ui->spinServiceQty->value());
    sv.setdg(ui->spinServicePrice->value());
    sv.setdv(ui->cbServiceName->currentText());
    int i=0;
    for (RoomServices &rsvs : roomservices){
        if (rsvs.getroom() == rsv.getroom()){
            rsvs.addservice(sv);
            i=1;
            break;
        }
    }
    if(!i){
        rsv.addservice(sv);
        roomservices.append(rsv);
    }
    saveRoomServices();
    showRoomServices();
    loadBill();
}
void MainWindow::showRoomServices() {
    QString phong = ui->cbServiceRoom->currentData().toString();
    ui->tableRoomServices->setRowCount(0);
    for (const RoomServices &rsv : roomservices)
    {
        QString name = rsv.getroom();
        if (name == phong) {
            QVector <Service> sv = rsv.getservice();
            for (const Service &svs : sv){
                double tt = svs.getdg() * svs.getsl();
                int row = ui->tableRoomServices->rowCount();
                ui->tableRoomServices->insertRow(row);

                ui->tableRoomServices->setItem(row,0,new QTableWidgetItem(svs.getdv()));
                ui->tableRoomServices->setItem(row,1,new QTableWidgetItem(QString::number(svs.getsl())));
               ui->tableRoomServices->setItem(row,2,new QTableWidgetItem(QString::number(svs.getdg())));
                ui->tableRoomServices->setItem(row,3,new QTableWidgetItem(QString::number(tt)));
            }
        }
    }
}
void MainWindow::on_cbServiceRoom_currentIndexChanged(int index){
    showRoomServices();
    loadBill();
}
void MainWindow::loadBill(){
    guest khachb;
    Room roomb;
    QString phong = ui->cbServiceRoom->currentData().toString();
    for (const guest &khach : guests){
        if (khach.getroom() == phong){
            khachb = khach;
            break;
        }
    }
    for (const Room &room : rooms){
        if (room.getroomNumber() == phong){
            roomb = room;
            break;
        }
    }
    ui->valStayGuest->setText(khachb.getgname());
    ui->valStayPhone->setText(khachb.getsdt());
    ui->valStayRoomType->setText(roomb.getroomType());
    ui->valStayRate->setText(QString::number(roomb.getroomprice()));
    ui->valStayCheckIn->setText(khachb.getnroom().toString("dd/MM/yyyy HH:mm"));
    ui->valStayPlannedCheckOut->setText(khachb.gettroom().toString("dd/MM/yyyy HH:mm"));
    ui->valStayCheckOut->setText(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm"));
    QDateTime nhan = khachb.getnroom();
    QDateTime tra  = QDateTime::currentDateTime();
    qint64 soNgay = nhan.date().daysTo(tra.date());
    if (tra.time() > QTime(12, 0)) soNgay += 1;
    if (soNgay < 1) soNgay = 1;
    ui->valStayNights->setText(QString::number(soNgay));
    double sum = 0;
    for (RoomServices &svs : roomservices){
        if (svs.getroom() == phong) {
            sum = svs.sum();
            break;
        }
    }
    ui->txtServiceCharge->setText(QString::number(sum));
    ui->txtPrepaidDeposit->setText(QString::number(khachb.gettiencoc()));
    ui->txtRoomCharge->setText(QString::number(soNgay*roomb.getroomprice(),'f',0));
    double discount = ui->spinDiscount->value();
    double tong = (soNgay*roomb.getroomprice() + sum) * (100 - discount)/100 - khachb.gettiencoc();
    ui->txtFinalTotal->setText(QString::number(tong,'f',0));
}
void MainWindow::on_spinDiscount_valueChanged(int index){
    loadBill();
}

