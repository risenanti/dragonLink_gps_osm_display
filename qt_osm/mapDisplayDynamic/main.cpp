
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
            if(serial.bytesAvailable()>25){
                input=serial.readAll();
                locData = processLocString(input.toStdString());
            }
        });

    CircleController circleController;
        circleController.setCenter(QGeoCoordinate(31.32, -89.29));
        circleController.setRadius(40);
    QTimer timer;

    QObject::connect(&timer, &QTimer::timeout, [&circleController](){
            circleController.translate(31.32, -89.29);//(double)locData.lat, (double)locData.longitude);
            //cout << locData.lat <<" " << locData.longitude << endl;
        });
    timer.start(1000);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("circleController", &circleController);
        engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
        if (engine.rootObjects().isEmpty()) return -1;

    return app.exec();
}
