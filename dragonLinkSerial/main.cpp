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
    float lat; float longitude;
};

coordinates processLocString(string input){
    coordinates result;
    auto startCur = input.find("=");
    auto endCur = input.find(",");
    if(startCur < 3) return result;
    string tempStr;
    for(auto i = startCur+1; i < endCur; ++i){
        tempStr+=input[i];
    }
    try{
        result.lat = stof(tempStr);
    }
    catch(exception a){
        result.lat = 0;
        result.longitude=0;
        return result;
    }

    startCur = input.find("=", startCur+1);
    endCur = input.find(",", endCur+1);
    tempStr.clear();
    if(startCur!=string::npos){
        if(endCur!=string::npos){
            for(auto i = startCur+1; i < endCur; ++i){
                    tempStr+=input[i];
                }

            try{
                result.longitude = stof(tempStr);
            }
            catch(exception a){
                result.lat = 0;
                result.longitude=0;
                return result;
            }
        }
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
    coordinates locData;
    QObject::connect(&serial, &QSerialPort::readyRead, [&]
    {
        if(serial.bytesAvailable()>28){
            input=serial.readLine();
            locData = processLocString(input.toStdString());
            cout << locData.lat << " " << locData.longitude << endl;
            //cout << input.toStdString() << endl;
            serial.flush();
        }
    });

    return a.exec();
}
