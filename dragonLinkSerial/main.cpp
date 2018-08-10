#include <QCoreApplication>
#include <QtSerialPort>
#include <QTimer>
#include <QString>

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <unistd.h>

using namespace std;
#define SERIALPORTNAME "ttyACM0"
#define SERIALBAUDRATE QSerialPort::Baud115200

//Latitude=32,Longitude=-18,END
string processString(QByteArray bytes){
    string result,temp = bytes.toStdString();



    return result;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QSerialPort serial;

    serial.setPortName(SERIALPORTNAME);
    serial.setBaudRate(SERIALBAUDRATE);

    if(!serial.open(QIODevice::ReadOnly)){
        cout <<"ERROR CANNOT OPEN SERIAL PORT" <<endl;
    }
    QByteArray input;

    while(true){
        serial.waitForReadyRead(-1);
        input.append(serial.readLine());
        serial.waitForReadyRead(-1);
        input.append(serial.readLine());
        serial.waitForReadyRead(-1);
        input.append(serial.readLine());
        cout << input.toStdString()<<endl;
        input.clear();


        input.clear();
        serial.flush();
        usleep(5*1000);
    }

    return a.exec();
}
