#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent*);
    ~MainWindow();
private slots:
    void Read_Data();
    void Warning_Cancel();
    void Start_Read();
private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QPushButton *butStart;
    QPushButton *butCancel;
    QDateTime *fireStart;
    QDateTime *fireStop;
};

#endif // MAINWINDOW_H
