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
string processString(string input){
    string result, temp;

    size_t found = input.find(",");

    for (auto i = 9; i < found; ++i){
        temp+=input[i];
    }
    for (auto i = temp.begin(); i < temp.end(); ++i){
        if ()
    }
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
        //cout << input.toStdString()<< "-------------PROG" <<endl;
        string inString = input.toStdString();
        cout << processString(inString) << endl;
        input.clear();


        input.clear();
        serial.flush();
        usleep(5*1000);
    }

    return a.exec();
}
