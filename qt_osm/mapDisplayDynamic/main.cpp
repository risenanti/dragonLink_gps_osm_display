
#include <QGeoCircle>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>
#include <QCoreApplication>
#include <QtSerialPort>
#include <QString>

#include <iostream>
#include "circlecontroller.h"

using namespace std;
#define SERIALPORTNAME "ttyACM0"
#define SERIALBAUDRATE QSerialPort::Baud115200

//Latitude=32,Longitude=-18,END
struct coordinates{
    float lat; float longitude;
};

coordinates processLocString(string input){
    coordinates result;
    if(input.size()<8){
        result.lat = 0;
        result.longitude=0;
        return result;
    }
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

coordinates locData;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QSerialPort serial;

    serial.setPortName(SERIALPORTNAME);
    serial.setBaudRate(SERIALBAUDRATE);

    if(!serial.open(QIODevice::ReadOnly)){
      cout <<"ERROR CANNOT OPEN SERIAL PORT" <<endl;
    }
    QByteArray input;


    QObject::connect(&serial, &QSerialPort::readyRead, [&]
        {
            if(serial.bytesAvailable()>28){
                input=serial.readLine();
                cout <<input.toStdString()<<endl;
                coordinates tempData = processLocString(input.toStdString());
                if(tempData.lat>0 && tempData.lat < 300 && tempData.longitude>-100 && tempData.longitude < 50 && tempData.longitude!=0){
                    locData.lat = tempData.lat;
                    locData.longitude = tempData.longitude;
                }
                cout << locData.lat <<" " << locData.longitude << endl;
                serial.flush();
            }
        });

    CircleController circleController;
        circleController.setCenter(QGeoCoordinate(31.328178, -89.3353));
        circleController.setRadius(100);
    QTimer timer;

    QObject::connect(&timer, &QTimer::timeout, [&circleController](){
        circleController.setCenter(QGeoCoordinate(locData.lat, locData.longitude));
            //cout << locData.lat <<" " << locData.longitude << endl;

        });
    timer.start(800);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("circleController", &circleController);
        engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
        if (engine.rootObjects().isEmpty()) return -1;

    return app.exec();
}

