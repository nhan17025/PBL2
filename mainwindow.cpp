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
    QFile file3("hotel_style.qss");
    this->setWindowIcon(QIcon("hotel.ico"));
    ui->tabWidget->setIconSize(QSize(15, 15));
    ui->tabWidget->setTabIcon(0, QIcon("icons/rooms.png"));     // Quản Lý Sơ Đồ Phòng
    ui->tabWidget->setTabIcon(1, QIcon("icons/checkin.png"));   // Check-In
    ui->tabWidget->setTabIcon(2, QIcon("icons/billing.png"));   // Dịch Vụ Và Hóa Đơn
    ui->tabWidget->setTabIcon(3, QIcon("icons/report.png"));    // Thống Kê Và Báo Cáo
    ui->tabWidget->setTabIcon(4, QIcon("icons/history.png"));   // Lịch Sử Thanh Toán
    ui->tabWidget->setTabIcon(5, QIcon("icons/setting.png"));  // Quản Lý Giá Các Danh Mục
    if (file3.open(QFile::ReadOnly | QFile::Text)) {
        QString styleSheet = QString::fromUtf8(file3.readAll());
        qApp->setStyleSheet(styleSheet);
        file3.close();
    }
    ui->tableRooms->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableRoomTypePrice->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableRoomTypePrice->horizontalHeader()->setStretchLastSection(true);
    ui->tableServicePrice->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableServicePrice->horizontalHeader()->setStretchLastSection(true);
    ui->tableRoomServices->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableRoomServices->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableRoomServices->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tableRoomServices->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->tableReportDetail->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableHistory->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableHistory->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableBookings->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableCustomers->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    loadguest();
    loadRooms();
    loadAvaiRooms();
    loadORooms();
    loadRoomServices();
    showRoomServices();
    loadBill();
    loadcustomer();
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
    loadrevenue();
    showCustomers();
    ui->dtReportFrom->setDate(QDate::currentDate().addDays(-30));
    ui->dtReportTo->setDate(QDate::currentDate());
    ui->dtHistoryFrom->setDate(QDate::currentDate().addDays(-30));
    ui->dtHistoryTo->setDate(QDate::currentDate());
    ui->dtCheckIn->setDateTime(QDateTime::currentDateTime());
    ui->dtCheckIn->setEnabled(false);
    ui->dtCheckOut->setDateTime(QDateTime::currentDateTime().addDays(1));
    loadThongKe();
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
            if (ui->tableRooms->item(i, 4)->text() == "Đang có khách (Occupied)") {
                QMessageBox::warning(this, "Cảnh báo", "Khong the xoa phong dang co khach!");
                return;
            }
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
            for (Room &r : rooms) {
                if (r.getroomNumber() == roomnumber && roomnumber != rnm) {
                    QMessageBox::critical(this, "Lỗi", "Số phòng đã tồn tại!");
                    return;
                }
            }
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
    QString gname = ui->txtGuestName->text().trimmed();
    QString cccd = ui->txtGuestID->text().trimmed();
    QString sdt = ui->txtGuestPhone->text().trimmed();
    QString room = ui->cbSelectRoom->currentText();
    QDateTime  nroom = QDateTime::currentDateTime();
    QDateTime troom = ui->dtCheckOut->dateTime();
    double tiencoc = ui->spinDeposit->value();
    QString roomdata = ui->cbSelectRoom->currentData().toString();
    if (gname.isEmpty() || cccd.isEmpty() || sdt.isEmpty()) {
        QMessageBox::warning(this, "Thiếu thông tin", "Vui lòng nhập đầy đủ Tên khách hàng, SDT và số CCCD trước khi nhận phòng");
        return;
    }
    bool cccdHopLe = (cccd.length() == 12);
    for (QChar ch : cccd) {
        if (!ch.isDigit()) cccdHopLe = false;
    }
    if (!cccdHopLe) {
        QMessageBox::warning(this, "Sai CCCD", "Số CCCD phải gồm đúng 12 chữ số!");
        return;
    }
    bool sdtHopLe = (sdt.length() == 10 && sdt[0] == '0');
    for (QChar ch : sdt) {
        if (!ch.isDigit()) sdtHopLe = false;
    }
    if (!sdtHopLe) {
        QMessageBox::warning(this, "Sai SĐT", "Số điện thoại phải gồm đúng 10 chữ số và bắt đầu bằng 0!");
        return;
    }
    if (roomdata.isEmpty()) {
        QMessageBox::warning(this, "Chưa có phòng", "Không có phòng trống nào được chọn. Vui lòng chọn phòng trước khi nhận phòng!");
        return;
    }
    if (troom <= nroom) {
        QMessageBox::warning(this, "Sai ngày", "Ngày trả phòng dự kiến phải sau thời điểm nhận phòng!");
        return;
    }
    for (const guest &k : guests) {
        if (k.getcccd() == cccd) {
            QMessageBox::warning(this, "Trùng CCCD", "Số CCCD này đang ở phòng " + k.getroom() + "!");
            return;
        }
        if (k.getsdt() == sdt) {
            QMessageBox::warning(this, "Trùng SĐT", "Số điện thoại này đang ở phòng " + k.getroom() + "!");
            return;
        }
    }
    bool daCo = false;
    for (customer &c : danhba) {
        if (c.getCccd() == cccd) {
            c.setName(gname);
            c.setSdt(sdt);
            daCo = true;
            break;
        }
    }
    if (!daCo) {
        danhba.append(customer(gname, cccd, sdt));
    }
    savecustomer();
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
    ui->tableBookings->setItem(row,3,new QTableWidgetItem(roomdata));
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
    ui->dtCheckIn->setDateTime(QDateTime::currentDateTime());
    ui->dtCheckOut->setDateTime(QDateTime::currentDateTime().addDays(1));
    saveRooms();
    loadRooms();
    loadAvaiRooms();
    saveguest();
    loadORooms();
    showCustomers();
}
void MainWindow::on_btnAddService_clicked(){
    if (ui->cbServiceRoom->count() == 0) {
        QMessageBox::warning(this, "Không có khách", "Hiện không có phòng nào đang có khách!");
        return;
    }
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
    ui->valStayCheckOut->setDateTime(QDateTime::currentDateTime());
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
    QDateTime nhan = khachb.getnroom();
    QDateTime tra  = ui->valStayCheckOut->dateTime();
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
    double tong = (soNgay*roomb.getroomprice() + sum) * (100 - discount)/100 - khachb.gettiencoc();
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
    if (ui->cbServiceRoom->count() == 0) {
        QMessageBox::warning(this, "Không có khách", "Hiện không có phòng nào đang có khách để thanh toán!");
        return;
    }
    if (phong.isEmpty()) {
        QMessageBox::warning(this, "Chưa chọn phòng", "Vui lòng chọn phòng cần thanh toán!");
        return;
    }
    QDateTime gioNhan = QDateTime::fromString(ui->valStayCheckIn->text(), "dd/MM/yyyy HH:mm");
    if (ui->valStayCheckOut->dateTime() < gioNhan) {
        QMessageBox::warning(this, "Sai giờ trả phòng", "Giờ trả phòng thực tế không được trước giờ nhận phòng!");
        return;
    }
    double tongTien = ui->txtFinalTotal->text().toDouble();
    QString phuongThuc = ui->cbPaymentMethod->currentText();
    PaymentMethod* payment = nullptr;
    if (phuongThuc == "Tiền mặt") {
        payment = new CashPayment();
    } else if (phuongThuc == "Chuyển khoản") {
        payment = new TransferPayment();
    } else if (phuongThuc == "Thẻ (POS)") {
        payment = new CardPayment();
    }
    if (payment == nullptr) {
        QMessageBox::warning(this, "Lỗi", "Phương thức thanh toán không hợp lệ!");
        return;
    }
    bool thanhcong = payment->processPayment(tongTien);
    delete payment;
    if (!thanhcong) {
        return;
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
    newBill.setCheckOut(ui->valStayCheckOut->dateTime());
    newBill.setRoomCharge(ui->txtRoomCharge->text().toDouble());
    newBill.setServiceCharge(ui->txtServiceCharge->text().toDouble());
    newBill.setDiscount(ui->spinDiscount->value());
    newBill.setTotalAmount(tongTien + ui->txtPrepaidDeposit->text().toDouble());
    newBill.setPaymentMethod(phuongThuc);
    histories.append(newBill);
    saveHistory();
    saverevenue();
    showHistory();
    loadrevenue();
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
    QString thongBao = "Phòng " + phong + " đã thanh toán bằng " + phuongThuc + ".\n";
    if (tongTien >= 0) {
        thongBao += "Số tiền thu: " + QString::number(tongTien, 'f', 0) + " VNĐ\n";
    } else {
        thongBao += "Trả lại khách: " + QString::number(-tongTien, 'f', 0) + " VNĐ\n";
    }
    thongBao += "Hóa đơn đã được lưu vào Lịch sử thanh toán.";
    QMessageBox::information(this, "Thanh toán thành công", thongBao);
    showCustomers();
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
void MainWindow::on_valStayCheckOut_dateTimeChanged(const QDateTime &) {
    loadBill();
}
void MainWindow::saverevenue(){
    revenueList.clear();
    for (const History &his : histories){
        bool check = false;
        for (Revenue &rev : revenueList) {
            if (his.getCheckOut().date() == rev.getDate()) {
                rev.addInvoice(his);
                check = true;
                break;
            }
        }
        if (check) continue;
        Revenue rev(his.getCheckOut().date());
        rev.addInvoice(his);
        revenueList.append(rev);
    }
    QJsonArray array;
    for (Revenue &rev : revenueList){
        QJsonObject ob;
        ob["Date"] = rev.getDate().toString("dd/MM/yyyy");;
        ob["Number"] = rev.getInvoiceCount();
        ob["RoomRevenue"] = rev.getRoomRevenue();
        ob["ServiceRevenue"] = rev.getServiceRevenue();
        ob["Total"] = rev.getTotalRevenue();
        array.append(ob);
    }
    QJsonDocument doc(array);
    QFile file ("revenue.json");
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(doc.toJson());
        file.close();
    }
}
void MainWindow::loadrevenue() {
    QFile file("revenue.json");
    if (!file.open(QIODevice::ReadOnly)) return;
    QByteArray data = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) return;
    QJsonArray array = doc.array();
    revenueList.clear();
    for (const QJsonValue &value : array) {
        QJsonObject obj = value.toObject();
        Revenue rev(QDate::fromString(obj["Date"].toString(), "dd/MM/yyyy"));
        rev.setInvoiceCount(obj["Number"].toInt());
        rev.setRoomRevenue(obj["RoomRevenue"].toDouble());
        rev.setServiceRevenue(obj["ServiceRevenue"].toDouble());
        rev.setTotalRevenue(obj["Total"].toDouble());
        revenueList.append(rev);
    }
    showrevenue();
}
void MainWindow::showrevenue() {
    ui->tableReportDetail->setRowCount(0);
    for (const Revenue &rev : revenueList) {
        int row = ui->tableReportDetail->rowCount();
        ui->tableReportDetail->insertRow(row);
        ui->tableReportDetail->setItem(row, 0, new QTableWidgetItem(rev.getDate().toString("dd/MM/yyyy")));
        ui->tableReportDetail->setItem(row, 1, new QTableWidgetItem(QString::number(rev.getInvoiceCount())));
        ui->tableReportDetail->setItem(row, 2, new QTableWidgetItem(QString::number(rev.getRoomRevenue(), 'f', 0)));
        ui->tableReportDetail->setItem(row, 3, new QTableWidgetItem(QString::number(rev.getServiceRevenue(), 'f', 0)));
        ui->tableReportDetail->setItem(row, 4, new QTableWidgetItem(QString::number(rev.getTotalRevenue(), 'f', 0)));
    }
}
void MainWindow::savecustomer(){
    QJsonArray array;
    for (customer &nguoi : danhba){
        QJsonObject ob;
        ob["HoTen"] = nguoi.getName();
        ob["CCCD"] = nguoi.getCccd();
        ob["SDT"] = nguoi.getSdt();
        array.append(ob);
    }
    QJsonDocument doc(array);
    QFile file ("danhba.json");
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(doc.toJson());
        file.close();
    }
}
void MainWindow::loadcustomer() {
    QFile file("danhba.json");
    if (!file.open(QIODevice::ReadOnly)) return;
    QByteArray data = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) return;
    QJsonArray array = doc.array();
    danhba.clear();
    for (const QJsonValue &value : array) {
        QJsonObject obj = value.toObject();
        customer nguoi(obj["HoTen"].toString(), obj["CCCD"].toString(), obj["SDT"].toString());
        danhba.append(nguoi);
    }
}
void MainWindow::on_txtGuestID_textChanged() {
    QString cccd = ui->txtGuestID->text().trimmed();
    if (cccd.length() == 12) {
        for (const customer &c : danhba) {
            if (c.getCccd() == cccd) {
                ui->txtGuestName->setText(c.getName());
                ui->txtGuestPhone->setText(c.getSdt());
                statusBar()->showMessage("Khách quen: đã tự điền tên và số điện thoại từ danh bạ.", 5000);
                return;
            }
        }
    }
}
void MainWindow::on_btnGenerateReport_clicked() {
    loadThongKe();
}
void MainWindow::loadThongKe() {
    QDate fromDate = ui->dtReportFrom->date();
    QDate toDate = ui->dtReportTo->date();

    if (fromDate > toDate) {
        QMessageBox::warning(this, "Sai ngày", "Ngày bắt đầu không được lớn hơn ngày kết thúc!");
        return;
    }
    double tongDoanhThu = 0;
    int luotKhach = 0;
    ui->tableReportDetail->setRowCount(0);
    for (const Revenue &rev : revenueList) {
        if (rev.getDate() >= fromDate && rev.getDate() <= toDate) {
            int row = ui->tableReportDetail->rowCount();
            ui->tableReportDetail->insertRow(row);
            ui->tableReportDetail->setItem(row, 0, new QTableWidgetItem(rev.getDate().toString("dd/MM/yyyy")));
            ui->tableReportDetail->setItem(row, 1, new QTableWidgetItem(QString::number(rev.getInvoiceCount())));
            ui->tableReportDetail->setItem(row, 2, new QTableWidgetItem(QString::number(rev.getRoomRevenue(), 'f', 0)));
            ui->tableReportDetail->setItem(row, 3, new QTableWidgetItem(QString::number(rev.getServiceRevenue(), 'f', 0)));
            ui->tableReportDetail->setItem(row, 4, new QTableWidgetItem(QString::number(rev.getTotalRevenue(), 'f', 0)));

            tongDoanhThu += rev.getTotalRevenue();
            luotKhach += rev.getInvoiceCount();
        }
    }
    int soNgayBaoCao = fromDate.daysTo(toDate) + 1;
    int tongCongSuat = rooms.size() * soNgayBaoCao;
    int tongDemDaBan = 0;

    if (tongCongSuat > 0) {
        for (const History &his : histories) {
            QDate checkIn = his.getCheckIn().date();
            QDate checkOut = his.getCheckOut().date();
            if (checkOut >= fromDate && checkIn <= toDate) {
                QDate start = (checkIn > fromDate) ? checkIn : fromDate;
                QDate end = (checkOut < toDate) ? checkOut : toDate;
                int soDem = start.daysTo(end);
                if (soDem == 0) soDem = 1;
                tongDemDaBan += soDem;
            }
        }
    }
    double tyLeLapDay = 0.0;
    if (tongCongSuat > 0) {
        tyLeLapDay = (static_cast<double>(tongDemDaBan) / tongCongSuat) * 100.0;
        if (tyLeLapDay > 100.0) tyLeLapDay = 100.0;
    }
    ui->lblTotalRevenueVal->setText(QString::number(tongDoanhThu, 'f', 0) + " VNĐ");
    ui->lblOccupancyVal->setText(QString::number(tyLeLapDay, 'f', 1) + " %");
    ui->lblGuestCountVal->setText(QString::number(luotKhach) + " lượt");
}
void MainWindow::showCustomers() {
    ui->tableCustomers->setRowCount(0);
    for (const customer &c : danhba) {
        int soLan = 0;
        QDateTime lanCuoi;
        for (const History &his : histories) {
            if (his.getCccd() == c.getCccd()) {
                soLan++;
                if (!lanCuoi.isValid() || his.getCheckOut() > lanCuoi) lanCuoi = his.getCheckOut();
            }
        }
        QString trangThai = "";
        for (const guest &k : guests) {
            if (k.getcccd() == c.getCccd()) {
                trangThai = "Đang ở phòng " + k.getroom();
                break;
            }
        }
        int row = ui->tableCustomers->rowCount();
        ui->tableCustomers->insertRow(row);
        ui->tableCustomers->setItem(row, 0, new QTableWidgetItem(c.getCccd()));
        ui->tableCustomers->setItem(row, 1, new QTableWidgetItem(c.getName()));
        ui->tableCustomers->setItem(row, 2, new QTableWidgetItem(c.getSdt()));
        ui->tableCustomers->setItem(row, 3, new QTableWidgetItem(QString::number(soLan)));
        ui->tableCustomers->setItem(row, 4, new QTableWidgetItem(lanCuoi.isValid() ? lanCuoi.toString("dd/MM/yyyy HH:mm") : ""));
        ui->tableCustomers->setItem(row, 5, new QTableWidgetItem(trangThai));
    }
    ui->lblCustomerCount->setText("Tổng số khách: " + QString::number(danhba.size()));
    on_txtSearchCustomer_textChanged(ui->txtSearchCustomer->text());
}
void MainWindow::on_txtSearchCustomer_textChanged(const QString &text) {
    QString tuKhoa = text.trimmed();
    for (int i = 0; i < ui->tableCustomers->rowCount(); i++) {
        bool khop = tuKhoa.isEmpty();
        for (int j = 0; j <= 2 && !khop; j++) {
            if (ui->tableCustomers->item(i, j)->text().contains(tuKhoa, Qt::CaseInsensitive)) khop = true;
        }
        ui->tableCustomers->setRowHidden(i, !khop);
    }
}
void MainWindow::on_btnBookCustomer_clicked() {
    int row = ui->tableCustomers->currentRow();
    if (row < 0 || ui->tableCustomers->isRowHidden(row)) {
        QMessageBox::warning(this, "Chưa chọn khách", "Vui lòng chọn một khách trong danh bạ!");
        return;
    }
    if (!ui->tableCustomers->item(row, 5)->text().isEmpty()) {
        QMessageBox::warning(this, "Khách đang ở", "Khách này hiện " + ui->tableCustomers->item(row, 5)->text().toLower() + "!");
        return;
    }
    ui->tabWidget->setCurrentWidget(ui->tabBooking);
    ui->txtGuestID->setText(ui->tableCustomers->item(row, 0)->text());
    ui->cbSelectRoom->setFocus();
}
void MainWindow::on_btnDeleteCustomer_clicked() {
    int row = ui->tableCustomers->currentRow();
    if (row < 0 || ui->tableCustomers->isRowHidden(row)) {
        QMessageBox::warning(this, "Chưa chọn khách", "Vui lòng chọn một khách trong danh bạ!");
        return;
    }
    if (!ui->tableCustomers->item(row, 5)->text().isEmpty()) {
        QMessageBox::warning(this, "Không thể xóa", "Khách này đang ở khách sạn, không thể xóa!");
        return;
    }
    QString cccd = ui->tableCustomers->item(row, 0)->text();
    QString ten = ui->tableCustomers->item(row, 1)->text();
    if (QMessageBox::question(this, "Xác nhận", "Xóa khách " + ten + " (CCCD " + cccd + ") khỏi danh bạ?\nLịch sử thanh toán vẫn được giữ lại.") != QMessageBox::Yes) {
        return;
    }
    for (int i = 0; i < danhba.size(); i++) {
        if (danhba[i].getCccd() == cccd) {
            danhba.removeAt(i);
            break;
        }
    }
    savecustomer();
    showCustomers();
}
