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
struct coordinates{
    int lat; int longitude;
};

coordinates processLocString(string input){
    coordinates result;
    auto startCur = input.find("=");
    auto endCur = input.find(",");
    string tempStr;
    for(auto i = startCur+1; i < endCur; ++i){
        tempStr+=input[i];
    }
    result.lat = stoi(tempStr);

    startCur = input.find("=", startCur+1);
    endCur = input.find(",", endCur+1);
    tempStr.clear();
    for(auto i = startCur+1; i < endCur; ++i){
            tempStr+=input[i];
        }
    result.longitude = stoi(tempStr);

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
    coordinates locData;
    QObject::connect(&serial, &QSerialPort::readyRead, [&]
    {
        if(serial.bytesAvailable()>25){
            input=serial.readAll();
            locData = processLocString(input.toStdString());
            cout << locData.lat << " " << locData.longitude << endl;
        }
    });

    return a.exec();
}
