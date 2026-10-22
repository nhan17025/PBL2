#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w; // Khởi tạo cửa sổ chính
    w.show();     // Hiển thị cửa sổ lên màn hình
    return a.exec(); // Chạy vòng lặp sự kiện của ứng dụng
}