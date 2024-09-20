#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

struct Car {
    char brand[50];
    char model[50];
    unsigned int year;
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_btnWrite_clicked();

private:
    Ui::Widget *ui;
    void writeToSharedMemory(const Car &car);
};
#endif // WIDGET_H
