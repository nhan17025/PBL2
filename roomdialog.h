#ifndef ROOMDIALOG_H
#define ROOMDIALOG_H

#include <QDialog>
#include <QVector>

namespace Ui {
class RoomDialog;
}

class RoomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RoomDialog(bool editMode = false, QWidget *parent = nullptr);
    ~RoomDialog();
    QString getRoomNumber() const;
    QString getRoomType() const;
    int getFloor() const;
    double getPrice() const;
    QString getState() const;
    void setRoomData(QString &roomnumber, QString &roomtype, int floor, double price, QString &state);
private:
    Ui::RoomDialog *ui;
    bool editMode;
private slots:
    void on_btnAdd_clicked();
    void on_btnCancel_clicked();
};

#endif // ROOMDIALOG_H
