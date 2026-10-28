#include "mainwindow.h"
#include "roomdialog.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDateTime>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this); // Thiết lập các thành phần giao diện từ file .ui
    ui->tableRooms->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableRoomTypePrice->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableRoomTypePrice->horizontalHeader()->setStretchLastSection(true);
    ui->tableServicePrice->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableServicePrice->horizontalHeader()->setStretchLastSection(true);
    loadguest();
    loadRooms();
    loadAvaiRooms();
    loadORooms();
    loadRoomServices();
    showRoomServices();
    loadBill();
    QFile file("roomprices.json");
    if (!file.exists() || file.size() == 0) {
        initRoomPrices();
        saveRoomPrices();
    } else {
        loadRoomPrices();
    }
    QFile file2("serviceprice.json");
    if (!file2.exists() || file2.size() == 0) {
        initServicePrices();
        saveServicePrices();
    } else {
        loadServicePrices();
    }
    on_cbServiceName_currentIndexChanged(0);
    loadHistory();
    ui->dtHistoryFrom->setDate(QDate::currentDate().addDays(-30));
    ui->dtHistoryTo->setDate(QDate::currentDate());
    ui->dtCheckIn->setDateTime(QDateTime::currentDateTime());
    ui->dtCheckIn->setEnabled(false);
    ui->dtCheckOut->setDateTime(QDateTime::currentDateTime().addDays(1));
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
        QString tenKhach = "";
        if (state == "Đang có khách (Occupied)") {
            for (const guest &khach : guests) {
                if (khach.getroom() == roomnumber) {
                    tenKhach = khach.getgname();
                    break;
                }
            }
        }
        ui->tableRooms->setItem(row, 0, new QTableWidgetItem(roomnumber));
        ui->tableRooms->setItem(row, 1, new QTableWidgetItem(roomtype));
        ui->tableRooms->setItem(row, 2, new QTableWidgetItem(QString::number(floor)));
        ui->tableRooms->setItem(row, 3, new QTableWidgetItem(QString::number(price,'f',0)));
        ui->tableRooms->setItem(row, 4, new QTableWidgetItem(state));
        ui->tableRooms->setItem(row, 5, new QTableWidgetItem(tenKhach));
    }

}
void MainWindow::on_btnAddRoom_clicked()
{
    RoomDialog dialog(rooms,false, this);
    dialog.setRoomPrices(roomPrices);
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
    RoomDialog dialog(rooms,true, this);
    dialog.setRoomPrices(roomPrices);
    if (ui->tableRooms->rowCount() > 0) {
        int row = ui->tableRooms->currentRow();
        if (row < 0) return;
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
    if (gname.isEmpty() || cccd.isEmpty() || sdt.isEmpty()) {
        QMessageBox::warning(this, "Thiếu thông tin", "Vui lòng nhập đầy đủ Tên khách hàng, SDT và số CCCD trước khi nhận phòng");
        return;
    }
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
    ui->txtGuestName->clear();
    ui->txtGuestID->clear();
    ui->txtGuestPhone->clear();
    ui->spinDeposit->setValue(0);
    saveRooms();
    loadRooms();
    loadAvaiRooms();
    saveguest();
    loadORooms();
}
void MainWindow::on_btnAddService_clicked(){
    QString roomNumber = ui->cbServiceRoom->currentData().toString();
    if (roomNumber.isEmpty()) {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn phòng cần gọi dịch vụ!");
        return;
    }
    RoomServices rsv;
    Service sv;
    rsv.setroom(ui->cbServiceRoom->currentData().toString());
    sv.setsl(ui->spinServiceQty->value());
    sv.setdg(ui->spinServicePrice->text().toDouble());
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
    if (phong.isEmpty()) {
        ui->valStayGuest->clear();
        ui->valStayPhone->clear();
        ui->valStayRoomType->clear();
        ui->valStayRate->clear();
        ui->valStayCheckIn->clear();
        ui->valStayPlannedCheckOut->clear();
        ui->valStayCheckOut->clear();
        ui->valStayNights->clear();

        ui->txtRoomCharge->setText("0");
        ui->txtServiceCharge->setText("0");
        ui->txtPrepaidDeposit->setText("0");
        ui->txtFinalTotal->setText("0");
        ui->spinDiscount->setValue(0);
        return;
    }
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
    ui->valStayRate->setText(QString::number(roomb.getroomprice(),'f',0));
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
    ui->txtServiceCharge->setText(QString::number(sum,'f',0));
    ui->txtPrepaidDeposit->setText(QString::number(khachb.gettiencoc()));
    ui->txtRoomCharge->setText(QString::number(soNgay*roomb.getroomprice(),'f',0));
    double discount = ui->spinDiscount->value();
    double tong = ((soNgay*roomb.getroomprice() + sum) - khachb.gettiencoc()) * (100 - discount)/100;
    ui->txtFinalTotal->setText(QString::number(tong,'f',0));
}
void MainWindow::on_spinDiscount_valueChanged(int index){
    loadBill();
}
void MainWindow::initRoomPrices() {
    roomPrices.clear();
    roomPrices.append(RoomPrice("Phòng Đơn (Single)", 300000));
    roomPrices.append(RoomPrice("Phòng Đôi (Double)", 500000));
    roomPrices.append(RoomPrice("Phòng VIP / Suite", 1000000));
}
void MainWindow::saveRoomPrices() {
    QJsonArray array;
    for (const RoomPrice &roompr : roomPrices){
        QJsonObject ob;
        ob["roomType"] = roompr.getRoomType();
        ob["price"] = roompr.getPrice();
        array.append(ob);
    }
    QJsonDocument doc(array);
    QFile file ("roomprices.json");
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(doc.toJson());
        file.close();
    }
}
void MainWindow::loadRoomPrices(){
    QFile file ("roomprices.json");
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
    roomPrices.clear();
    for (const QJsonValue &value : array) {
        QJsonObject obj = value.toObject();
        RoomPrice roompr(obj["roomType"].toString(),obj["price"].toDouble());
        roomPrices.append(roompr);
    }
    ui->tableRoomTypePrice->setRowCount(0);
    for (const RoomPrice &roompr : roomPrices){
        int row = ui->tableRoomTypePrice->rowCount();
        ui->tableRoomTypePrice->insertRow(row);
        QString room = roompr.getRoomType();
        double price = roompr.getPrice();
        ui->tableRoomTypePrice->setItem(row, 0, new QTableWidgetItem(room));
        ui->tableRoomTypePrice->setItem(row, 1, new QTableWidgetItem(QString::number(price, 'f', 0)));
    }
}
void MainWindow::on_btnUpdateRoomType_clicked(){
    double price = ui->spinRoomTypePrice->value();
    QString roomType = ui->cbRoomTypeName->currentText();
    for (RoomPrice &roompr : roomPrices){
        if (roompr.getRoomType() == roomType) {
            roompr.setPrice(price);
            saveRoomPrices();
            loadRoomPrices();
            for (Room &room : rooms) {
                if (room.getroomType() == roomType) {
                    room.setprice(price);
                }
            }
            saveRooms();
            loadRooms();
            loadAvaiRooms();
            loadORooms();
            return;
        }
    }
}
void MainWindow::initServicePrices(){
    servicePrices.clear();
    servicePrices.append(serviceprice("Nước ngọt / Bia mini-bar",20000));
    servicePrices.append(serviceprice("Giặt ủi quần áo",15000));
    servicePrices.append(serviceprice("Bữa sáng tại phòng",50000));
    servicePrices.append(serviceprice("Dịch vụ xe đưa đón sân bay",200000));
    servicePrices.append(serviceprice("Spa / Massage",120000));
}
void MainWindow::loadServicePrices(){
    QFile file ("serviceprice.json");
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
    servicePrices.clear();
    for (const QJsonValue &value : array) {
        QJsonObject obj = value.toObject();
        serviceprice svpr(obj["namedv"].toString(),obj["price"].toDouble());
        servicePrices.append(svpr);
    }
    ui->tableServicePrice->setRowCount(0);
    for (const serviceprice &svpr : servicePrices){
        int row = ui->tableServicePrice->rowCount();
        ui->tableServicePrice->insertRow(row);
        QString dv = svpr.getnamedv();
        double price = svpr.getprice();
        ui->tableServicePrice->setItem(row, 0, new QTableWidgetItem(dv));
        ui->tableServicePrice->setItem(row, 1, new QTableWidgetItem(QString::number(price, 'f', 0)));
    }
}
void MainWindow::saveServicePrices() {
    QJsonArray array;
    for (const serviceprice &svpr : servicePrices){
        QJsonObject ob;
        ob["namedv"] = svpr.getnamedv();
        ob["price"] = svpr.getprice();
        array.append(ob);
    }
    QJsonDocument doc(array);
    QFile file ("serviceprice.json");
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(doc.toJson());
        file.close();
    }
}
void MainWindow::on_btnUpdateServiceCat_clicked(){
    double price = ui->spinServiceCatPrice->value();
    QString sv = ui->cbServiceCatName->currentText();
    for (serviceprice &svpr : servicePrices){
        if (svpr.getnamedv() == sv) {
            svpr.setprice(price);
            saveServicePrices();
            loadServicePrices();
            for (RoomServices &svs : roomservices){
                svs.updateServicePrice(sv,price);
            }
            saveRoomServices();
            loadRoomServices();
            showRoomServices();
            loadBill();
            QString tenDichVu = ui->cbServiceName->currentText();
            for (const serviceprice &svpr : servicePrices) {
                if (svpr.getnamedv() == tenDichVu) {
                    ui->spinServicePrice->setText(QString::number(svpr.getprice(), 'f', 0));
                    break;
                }
            }
            return;
        }
    }
}
void MainWindow::on_cbServiceName_currentIndexChanged(int)
{
    QString tenDichVu = ui->cbServiceName->currentText();
    for (const serviceprice &svpr : servicePrices) {
        if (svpr.getnamedv() == tenDichVu) {
            ui->spinServicePrice->setText(QString::number(svpr.getprice(), 'f', 0));
            break;
        }
    }
}
void MainWindow::on_btnCheckOutAndPrint_clicked() {
    QString phong = ui->cbServiceRoom->currentData().toString();
    if (phong.isEmpty()) return;
    double tongTien = ui->txtFinalTotal->text().toDouble();
    QString phuongThuc = ui->cbPaymentMethod->currentText();
    PaymentMethod* payment = nullptr;
    if (phuongThuc == "Tiền mặt") {
        payment = new CashPayment();
    } else if (phuongThuc == "Chuyển khoản") {
        payment = new TransferPayment();
    }
    if (payment != nullptr) {
        bool thanhcong = payment->processPayment(tongTien);
        delete payment;
        if (!thanhcong) {
            return;
        }
    }
    QString cccdKhach;
    for (const guest &k : guests) {
        if (k.getroom() == phong) {
            cccdKhach = k.getcccd();
            break;
        }
    }
    History newBill;
    newBill.setGuestName(ui->valStayGuest->text());
    newBill.setCccd(cccdKhach);
    newBill.setRoomNumber(phong);
    newBill.setCheckIn(QDateTime::fromString(ui->valStayCheckIn->text(), "dd/MM/yyyy HH:mm"));
    newBill.setCheckOut(QDateTime::currentDateTime());
    newBill.setRoomCharge(ui->txtRoomCharge->text().toDouble());
    newBill.setServiceCharge(ui->txtServiceCharge->text().toDouble());
    newBill.setDiscount(ui->spinDiscount->value());
    newBill.setTotalAmount(tongTien);
    newBill.setPaymentMethod(phuongThuc);
    histories.append(newBill);
    saveHistory();
    showHistory();
    for (int i = 0; i < roomservices.size(); ++i) {
        if (roomservices[i].getroom() == phong) {
            roomservices.removeAt(i);
            break;
        }
    }
    for (int i = 0; i < guests.size(); ++i) {
        if (guests[i].getroom() == phong) {
            guests.removeAt(i);
            break;
        }
    }
    for (Room &room : rooms) {
        if (room.getroomNumber() == phong) {
            room.setstate("Đang dọn dẹp (Cleaning)");
            break;
        }
    }
    saveRooms();
    saveguest();
    saveRoomServices();
    loadRooms();
    loadguest();
    loadORooms();
    loadAvaiRooms();
    ui->cbServiceRoom->setCurrentIndex(-1);
    ui->tableRoomServices->setRowCount(0);
}
void MainWindow::saveHistory() {
    QJsonArray array;
    for (const History &his : histories) {
        QJsonObject ob;
        ob["guestName"] = his.getGuestName();
        ob["cccd"] = his.getCccd();
        ob["roomNumber"] = his.getRoomNumber();
        ob["checkIn"] = his.getCheckIn().toString("dd/MM/yyyy HH:mm");
        ob["checkOut"] = his.getCheckOut().toString("dd/MM/yyyy HH:mm");
        ob["roomCharge"] = his.getRoomCharge();
        ob["serviceCharge"] = his.getServiceCharge();
        ob["discount"] = his.getDiscount();
        ob["totalAmount"] = his.getTotalAmount();
        ob["paymentMethod"] = his.getPaymentMethod();
        ob["note"] = his.getNote();
        array.append(ob);
    }
    QJsonDocument doc(array);
    QFile file("history.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}

void MainWindow::loadHistory() {
    QFile file("history.json");
    if (!file.open(QIODevice::ReadOnly)) return;

    QByteArray data = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) return;

    QJsonArray array = doc.array();
    histories.clear();
    for (const QJsonValue &value : array) {
        QJsonObject obj = value.toObject();
        History his;
        his.setGuestName(obj["guestName"].toString());
        his.setCccd(obj["cccd"].toString());
        his.setRoomNumber(obj["roomNumber"].toString());
        his.setCheckIn(QDateTime::fromString(obj["checkIn"].toString(), "dd/MM/yyyy HH:mm"));
        his.setCheckOut(QDateTime::fromString(obj["checkOut"].toString(), "dd/MM/yyyy HH:mm"));
        his.setRoomCharge(obj["roomCharge"].toDouble());
        his.setServiceCharge(obj["serviceCharge"].toDouble());
        his.setDiscount(obj["discount"].toDouble());
        his.setTotalAmount(obj["totalAmount"].toDouble());
        his.setPaymentMethod(obj["paymentMethod"].toString());
        his.setNote(obj["note"].toString());
        histories.append(his);
    }
    showHistory();
}
void MainWindow::showHistory() {
    ui->tableHistory->setRowCount(0);
    double tongDoanhThu = 0;
    for (const History &his : histories) {
        int row = ui->tableHistory->rowCount();
        ui->tableHistory->insertRow(row);
        ui->tableHistory->setItem(row, 0, new QTableWidgetItem(his.getGuestName()));
        ui->tableHistory->setItem(row, 1, new QTableWidgetItem(his.getCccd()));
        ui->tableHistory->setItem(row, 2, new QTableWidgetItem(his.getRoomNumber()));
        ui->tableHistory->setItem(row, 3, new QTableWidgetItem(his.getCheckIn().toString("dd/MM/yyyy HH:mm")));
        ui->tableHistory->setItem(row, 4, new QTableWidgetItem(his.getCheckOut().toString("dd/MM/yyyy HH:mm")));
        ui->tableHistory->setItem(row, 5, new QTableWidgetItem(QString::number(his.getRoomCharge(), 'f', 0)));
        ui->tableHistory->setItem(row, 6, new QTableWidgetItem(QString::number(his.getServiceCharge(), 'f', 0)));
        ui->tableHistory->setItem(row, 7, new QTableWidgetItem(QString::number(his.getDiscount(), 'f', 0)));
        ui->tableHistory->setItem(row, 8, new QTableWidgetItem(QString::number(his.getTotalAmount(), 'f', 0)));
        ui->tableHistory->setItem(row, 9, new QTableWidgetItem(his.getPaymentMethod()));
        tongDoanhThu += his.getTotalAmount();
    }
    ui->lblHistoryCount->setText("Số hóa đơn: " + QString::number(histories.size()));
    ui->lblHistoryTotal->setText("Tổng thu: " + QString::number(tongDoanhThu, 'f', 0) + " VNĐ");
}
void MainWindow::on_btnFilterHistory_clicked() {
    QString search = ui->txtSearchHistory->text();
    QDate fromDate = ui->dtHistoryFrom->date();
    QDate toDate = ui->dtHistoryTo->date();
    int count = 0;
    double tongDoanhThu = 0;
    for (int i = ui->tableHistory->rowCount() - 1; i >= 0; i--) {
        QString chuoiNgay = ui->tableHistory->item(i, 4)->text();
        QDate ngayTra = QDateTime::fromString(chuoiNgay, "dd/MM/yyyy HH:mm").date();
        QString cccd = ui->tableHistory->item(i, 1)->text();
        if ((ui->tableHistory->item(i,1)->text() == search  || search == "") && (ngayTra >= fromDate) && (ngayTra <= toDate)) {
            ui->tableHistory->setRowHidden(i, false);
            count++;
            tongDoanhThu += ui->tableHistory->item(i, 8)->text().toDouble();
        } else {
            ui->tableHistory->setRowHidden(i, true);
        }
    }
    ui->lblHistoryCount->setText("Số hóa đơn: " + QString::number(count));
    ui->lblHistoryTotal->setText("Tổng thu: " + QString::number(tongDoanhThu, 'f', 0) + " VNĐ");
}
void MainWindow::on_btnRefreshHistory_clicked() {
    ui->txtSearchHistory->clear();
    ui->dtHistoryFrom->setDate(QDate::currentDate().addDays(-30));
    ui->dtHistoryTo->setDate(QDate::currentDate());
    showHistory();
}
void MainWindow::on_btnSetCleaning_clicked(){
    for (Room &room : rooms){
        if (room.getroomstate() == "Đang dọn dẹp (Cleaning)") {
            room.setstate("Trống (Available)");
        }
    }
    saveRooms();
    loadRooms();
    loadAvaiRooms();
}