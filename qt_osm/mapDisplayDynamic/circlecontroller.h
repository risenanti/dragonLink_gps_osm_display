#ifndef CIRCLECONTROLLER_H
#define CIRCLECONTROLLER_H
#include <QGeoCircle>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>

#include <random>

class CircleController: public QObject{
    Q_OBJECT
    Q_PROPERTY(QGeoCoordinate center READ center NOTIFY centerChanged)
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)
public:
    CircleController(QObject *parent=0):QObject(parent){
        mCircle.setRadius(1000);
    }
    void setCenter(const QGeoCoordinate &center){
        if(mCircle.center() == center)
            return;
        mCircle.setCenter(center);
        emit centerChanged();
    }
    QGeoCoordinate center() const{ return mCircle.center();}

    void translate(double degreesLatitude, double degreesLongitude){
        mCircle.translate(degreesLatitude, degreesLongitude);
        emit centerChanged();
    }

    qreal radius() const{ return mCircle.radius();}
    void setRadius(const qreal &radius){
        if(mCircle.radius() == radius)
            return;
        mCircle.setRadius(radius);
        emit radiusChanged();
    }
signals:
    void centerChanged();
    void radiusChanged();
private:
    QGeoCircle mCircle;
};
#endif // CIRCLECONTROLLER_H
