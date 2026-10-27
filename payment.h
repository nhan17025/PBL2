#ifndef PAYMENT_H
#define PAYMENT_H

#include <QString>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>

class PaymentMethod {
public:
    virtual ~PaymentMethod() {}
    virtual QString getMethodName() const = 0;
    virtual bool processPayment(double amount) = 0;
};
class CashPayment : public PaymentMethod {
public:
    QString getMethodName() const override {
        return "Tiền mặt";
    }
    bool processPayment(double amount) override {
        return true;
    }
};
class TransferPayment : public PaymentMethod {
public:
    QString getMethodName() const override {
        return "Chuyển khoản";
    }
    bool processPayment(double amount) override {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Quét mã QR Thanh toán");
        msgBox.setText(QString("Khách hàng vui lòng quét mã QR để thanh toán:\nSố tiền cần chuyển: %1 VNĐ").arg(QString::number(amount, 'f', 0)));
        QPixmap qrPixmap("qr.jpg");
        if (!qrPixmap.isNull()) {
            msgBox.setIconPixmap(qrPixmap.scaled(250, 250, Qt::KeepAspectRatio));
        } else {
            msgBox.setInformativeText("(Không tải được ảnh mã QR)");
        }
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.button(QMessageBox::Ok)->setText("Đã nhận được tiền");
        msgBox.button(QMessageBox::Cancel)->setText("Chưa nhận được (Hủy)");
        int ret = msgBox.exec();
        if (ret == QMessageBox::Ok) {
            return true;
        }
        return false;
    }
};
#endif // PAYMENT_H