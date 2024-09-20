#include "widget.h"
#include "./ui_widget.h"
#include <QMessageBox>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_btnWrite_clicked()
{
    QString brand = ui->leBrand->text();
    QString model = ui->leModel->text();
    unsigned int year = ui->leYear->text().toUInt(); // toUInt() kullanın

    if (brand.isEmpty() || model.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill all fields !!!");
        return;
    }

    Car car;

    // QString'den C string'e kopyalama
    QByteArray brandArray = brand.toUtf8();
    QByteArray modelArray = model.toUtf8();

    strncpy(car.brand, brandArray.constData(), sizeof(car.brand) - 1);
    car.brand[sizeof(car.brand) - 1] = '\0'; // Null terminate

    strncpy(car.model, modelArray.constData(), sizeof(car.model) - 1);
    car.model[sizeof(car.model) - 1] = '\0'; // Null terminate

    car.year = year;

    writeToSharedMemory(car);
}

void Widget::writeToSharedMemory(const Car &car)
{
    // POSIX shared memory oluştur
    int shm_fd = shm_open("/my_shared_memory", O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        qDebug() << "Shared memory oluşturulamadı!";
        return;
    }

    // Hafızayı struct boyutuna göre ayarla
    if (ftruncate(shm_fd, sizeof(Car)) == -1) {
        qDebug() << "Shared memory boyutlandırılamadı!";
        ::close(shm_fd);
        return;
    }

    // Hafızayı map et
    void* ptr = mmap(0, sizeof(Car), PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        qDebug() << "Memory map başarısız!";
        ::close(shm_fd);
        return;
    }

    // Struct verisini paylaşılan hafızaya yaz
    memcpy(ptr, &car, sizeof(Car));

    // Belleği temizle
    munmap(ptr, sizeof(Car));
    ::close(shm_fd);
}
