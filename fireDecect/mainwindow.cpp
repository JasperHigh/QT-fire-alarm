#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QTimer>
#include<QPainter>
#include<QPen>
#include<QMessageBox>
#include<QtDebug>
#include<QDateTime>
//#include<QLineEdit>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->fireStart = NULL;
    this->fireStop=NULL;
    qDebug()<<"fireStart = "<<fireStart;
    ui->setupUi(this);
//    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()){
//        QSerialPort serial;
//        serial.setPort(info);
//        if(serial.open(QIODevice::ReadWrite)){

//        }
//    }
    //ui->butCancel->setEnabled(false);
    butStart=new QPushButton("开始测量");
    butStart->setParent(this);
    butStart->move(300,300);
    butStart->resize(93,28);
    butStart->show();
    connect(butStart,SIGNAL(clicked()),this,SLOT(Start_Read()));
    butCancel=new QPushButton("功能测试");
    butCancel->setParent(this);
    butCancel->move(400,300);
    butCancel->resize(93,28);
    butCancel->show();
    connect(butCancel,SIGNAL(clicked()),this,SLOT(Warning_Cancel()));
    butCancel->setEnabled(false);
//    fireStart=new QDateTime(123);

}

MainWindow::~MainWindow()
{
    serial->close();
    delete ui;
}
void MainWindow::Start_Read(){
    serial =new QSerialPort;
    serial->setPortName("COM15");
   if (serial->open(QIODevice::ReadWrite))
   {
       serial->setBaudRate(9600);
       serial->setDataBits(QSerialPort::Data8);
       serial->setParity(QSerialPort::NoParity);
       serial->setStopBits(QSerialPort::OneStop);
       serial->setFlowControl(QSerialPort::NoFlowControl);
       QTimer *t=new QTimer(this);//定义一个计时器
       connect(t,SIGNAL(timeout()),this,SLOT(Read_Data()));//每次计时器设定的时间间隔到了就触发读串口数据的函数
       t->start(1000);//将定时器时间间隔设定为1秒
       butStart->setEnabled(false);
       butCancel->setEnabled(true);
       ui->statusBar->showMessage("COM15连接成功",3000);
   }
   else{
       ui->statusBar->showMessage("COM15连接失败，请重试！",3000);
   }
}
void MainWindow::Read_Data(){
    QString strWave=serial->readLine();//读取第一行数据（火焰探测器发送的数据）
    qDebug()<<strWave;
    QString strTem=serial->readLine();//读取第二行数据发送的数据
    qDebug()<<strTem;
    if(strWave==""||strTem==""){//如果未读到值，则控制器处于蜂鸣和亮灯的一秒延时中，此时不更新数据
        return;
    }
    ui->textWave->setText(strWave);//为显示波长的文本框赋值
    ui->textTem->setText(strTem);//为显示温度的文本框赋值
    repaint();//重绘图表
    if(ui->textWave->text().toInt()>=550&&fireStart==NULL){//如果热源波长超过550且警报开始时间未初始化，说明警报开始
        this->fireStart = new QDateTime();
        this->fireStart->setDate(QDate::currentDate());
        this->fireStart->setTime(QTime::currentTime());

    }
    if(ui->textWave->text().toInt()<550&&fireStart!=NULL){//如果热源波长小于550且警报开始时间已初始化，说明警报结束
        fireStop = new QDateTime();
        fireStop->setDate(QDate::currentDate());
        fireStop->setTime(QTime::currentTime());
        ui->textWarning->append(fireStart->toString()+"至"+fireStop->toString()+"有火灾警报\n");
        int seconds=fireStart->time().msecsTo(fireStop->time())/1000;//计算警报开始到结束经过多少秒
        ui->textWarning->append("持续"+QString::number(seconds,10)+"秒\n");
        delete fireStart;
        delete fireStop;
        fireStart = fireStop = NULL;
    }
}
void MainWindow::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.translate(220,80);
    QPen pen(Qt::white,3,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
    painter.setPen(pen);
    painter.drawLine(0,40,0,-40);

    painter.drawLine(0,-30,320,-30);
    painter.drawLine(0,30,320,30);
    int num=1;
    int x,y;
    QPen pen2(Qt::black,2,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
    QPoint point;
    painter.setPen(pen2);
    for(;num<21;num++){
        x=15*num;
        if(num%2==0){
        y=-20;
        point.setX(x-8);
        point.setY(y+10);
        painter.drawText(point,QString::number(3*num));
}
        else{
        y=-25;
}
        painter.drawLine(x,-30,x,y);
}
    QLinearGradient linearGradient=QLinearGradient(0,0,300,0);
    linearGradient.setColorAt(0.0,Qt::cyan);
    linearGradient.setColorAt(0.6,Qt::yellow);
    linearGradient.setColorAt(1,Qt::red);
    QBrush qsh(linearGradient);
    painter.setBrush(qsh);
    painter.setPen(Qt::cyan);
    painter.drawRect(0,-5,(int)(ui->textTem->text().toFloat()/60*300),34);
//    QMessageBox::information(this,"information",ui->textTem->toPlainText().mid(0,ui->textTem->toPlainText().length()-1),QMessageBox::Ok);
//    qDebug()<<ui->textTem->toPlainText().toInt();


    QPainter painter2(this);
    painter2.translate(220,180);
    QPen pen3(Qt::white,3,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
    painter2.setPen(pen3);
    painter2.drawLine(0,40,0,-40);

    painter2.drawLine(0,-30,320,-30);
    painter2.drawLine(0,30,320,30);
    int num2=1;
    int x2,y2;
    QPen pen4(Qt::black,2,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
    QPoint point2;
    painter2.setPen(pen4);
    for(;num2<21;num2++){
        x2=15*num2;
        if(num2%2==0){
        y2=-20;
        point2.setX(x2-8);
        point2.setY(y2+10);
        painter2.drawText(point2,QString::number(55*num2));
}
        else{
        y2=-25;
}
        painter2.drawLine(x2,-30,x2,y2);
}
    QLinearGradient linearGradient2=QLinearGradient(0,0,300,0);
    linearGradient2.setColorAt(0.0,Qt::cyan);
    linearGradient2.setColorAt(0.6,Qt::yellow);
    linearGradient2.setColorAt(1,Qt::red);
    QBrush qsh2(linearGradient2);
    painter2.setBrush(qsh2);
    painter2.setPen(Qt::cyan);
    painter2.drawRect(0,-5,(int)(ui->textWave->text().toFloat()/1100*300),34);
//    qDebug()<<"shit";


}


void MainWindow::Warning_Cancel(){
    serial->write("0");
}
