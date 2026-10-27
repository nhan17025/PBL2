#include "roomdialog.h"
#include "ui_roomdialog.h"
#include <QMessageBox>

RoomDialog::RoomDialog(const QVector<Room> &rooms,bool editMode, QWidget *parent)
    : QDialog(parent)
    , rooms(rooms)
    , ui(new Ui::RoomDialog)
    , editMode(editMode)
{
    ui->setupUi(this);
    if (editMode) {
        ui->btnAdd->setText("Save");
    }
}

RoomDialog::~RoomDialog()
{
    delete ui;
}
QString RoomDialog::getRoomNumber() const {
    return ui->txtRoomNumber->text();
}
QString RoomDialog::getRoomType() const {
    return ui->cbRoomType->currentText();
}
int RoomDialog::getFloor() const {
    return ui->spinFloor->value();
}
double RoomDialog::getPrice() const {
    return ui->spinPrice->text().toDouble();
}
QString RoomDialog::getState() const {
    return ui->cbState->currentText();
}
void RoomDialog::on_btnAdd_clicked(){
    QString roomNumber = ui->txtRoomNumber->text().trimmed();

    if (roomNumber.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập số phòng!");
        return;
    }
    for (const Room &room : rooms) {
        if (room.getroomNumber() == roomNumber && editMode == false) {
            QMessageBox::warning(this, "Lỗi",
                                 "Số phòng đã tồn tại!");
            return;
        }
    }
    accept();
}
void RoomDialog::on_btnCancel_clicked(){
    reject();
}
void RoomDialog::setRoomData(QString &roomnumber, QString &roomtype, int floor, double price, QString &state) {
    ui->txtRoomNumber->setText(roomnumber);
    ui->cbRoomType->setCurrentText(roomtype);
    ui->spinFloor->setValue(floor);
    ui->spinPrice->setText(QString::number(price, 'f', 0));
    int index = ui->cbState->findText(state);
    if (index != -1) {
        ui->cbState->setCurrentIndex(index);
    } else {
        ui->cbState->addItem(state);
        ui->cbState->setCurrentText(state);
    }
    if (state == "Đang có khách (Occupied)") {
        ui->cbState->setEnabled(false);
        ui->txtRoomNumber->setEnabled(false);
        ui->cbRoomType->setEnabled(false);
        ui->spinFloor->setEnabled(false);
    } else {
        ui->cbState->setEnabled(true);
        ui->txtRoomNumber->setEnabled(true);
        ui->cbRoomType->setEnabled(true);
        ui->spinFloor->setEnabled(true);
    }
}
void RoomDialog::setRoomPrices(const QVector<RoomPrice>& p) {
    prices = p;
    updateRoomPrice();
}
void RoomDialog::on_cbRoomType_currentIndexChanged(int) {
    updateRoomPrice();
}
void RoomDialog::updateRoomPrice() {
    QString roomType = ui->cbRoomType->currentText();
    double price = 0;
    for (const RoomPrice &rp : prices) {
        if (rp.getRoomType() == roomType) { price = rp.getPrice(); break; }
    }
    ui->spinPrice->setText(QString::number(price, 'f', 0));
}