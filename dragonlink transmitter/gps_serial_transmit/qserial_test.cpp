#include <QCoreApplication>
#include <iostream>
#include <QSerialPort>
#include <QDebug>
#include <QElapsedTimer>

using namespace std;
QSerialPort serial;
QElapsedTimer timer;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    serial.setPortName("ttyUSB0");
    serial.open(QIODevice::ReadWrite);

    serial.setBaudRate(QSerialPort::Baud115200);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    while(!serial.isOpen()) serial.open(QIODevice::ReadWrite);

    if (serial.isOpen() && serial.isWritable())
    {
       qDebug() << "Serial is open";

       QByteArray output;
       QByteArray input;

       while(true)
       {
         output = "a";
         serial.write(output);
        serial.flush();
        timer.start();
        qDebug() << timer.elapsed();

         serial.waitForBytesWritten(100);
         serial.waitForReadyRead(100);

        if(serial.bytesAvailable()>=18)
         input = serial.read(18);
        qDebug()<<input;
         }
       }
    return a.exec();
}
