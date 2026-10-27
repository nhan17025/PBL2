#include "roomdialog.h"
#include "ui_roomdialog.h"

RoomDialog::RoomDialog(bool editMode, QWidget *parent)
    : QDialog(parent)
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
    }
}