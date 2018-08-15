import QtQuick 2.6
import QtQuick.Window 2.2
import QtLocation 5.6
import QtPositioning 5.6

Window {
    visible: true
    width: 1024
    height: 768

    Plugin {
        id: osmMapPlugin
        name: "osm"
    }
    Map {
        anchors.fill: parent
        plugin: osmMapPlugin
        center: QtPositioning.coordinate(31.32, -89.29)
        zoomLevel: 13
        MapCircle {
            center: circleController.center
            radius: circleController.radius
            color: 'green'
            border.width: 0.05
        }
    }
}
