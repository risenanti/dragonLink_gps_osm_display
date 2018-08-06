
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

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QSerialPort m_serialPort;
    m_serialPort.setPortName(SERIALPORTNAME);
    m_serialPort.setBaudRate(SERIALBAUDRATE);

    if(!m_serialPort.open(QIODevice::ReadWrite)){
             cout << "ERROR CANNOT OPEN SERIAL PORT"<<endl;
             return -1;
             }

    QByteArray input;

    CircleController circleController;
        circleController.setCenter(QGeoCoordinate(31.3271189, -89.2903392));
        circleController.setRadius(20);
    QTimer timer;

    QObject::connect(&timer, &QTimer::timeout, [&circleController](){
 /*           while(m_serialPort.canReadLine()){
                input = m_serialPort.readLine();
            }

            string txString = input.toStdString();

            circleController.translate(dis(e), dis(e)); */
        });
    timer.start(1000);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("circleController", &circleController);
        engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
        if (engine.rootObjects().isEmpty()) return -1;

    return app.exec();
}
