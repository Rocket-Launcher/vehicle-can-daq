import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Extras 1.4
import QtQuick.Controls.Styles 1.4
import "ColorFormatter.js" as CF


Window {
    id: root
    visible: true
    width: 800
    height: 480
    title: "Linear Cluster"
    color: "#000"

    Item {
        id: container
        x: 0
        y: 0
        width: 800
        height: Math.min(root.width, root.height)
        visible: true
        z: 0
        rotation: 0
        scale: 1
        anchors.verticalCenterOffset: 0
        anchors.horizontalCenterOffset: 0
        anchors.centerIn: parent

        // Animation for Tachometer.
        Timer {
            running: true
            repeat: true
            interval: 4000
            onTriggered: tachGauge.value = tachGauge.value == tachGauge.maximumValue ? 0 : tachGauge.maximumValue
        }

        // Animation for Fuel Gauge.
        Timer {
            running: true
            repeat: true
            interval: 4500
            onTriggered: fuelGauge.value = fuelGauge.value == fuelGauge.maximumValue ? 0 : fuelGauge.maximumValue
        }

        // Animation for Coolant Temp Gauge.
        Timer {
            running: true
            repeat: true
            interval: 4500
            onTriggered: coolantTempGauge.value = coolantTempGauge.value == coolantTempGauge.maximumValue ? 0 : coolantTempGauge.maximumValue
        }

        // Animation for Oil Temp Gauge.
        Timer {
            running: true
            repeat: true
            interval: 4500
            onTriggered: oilTempGauge.value = oilTempGauge.value == oilTempGauge.maximumValue ? 0 : oilTempGauge.maximumValue
        }



        Gauge {
            id: tachGauge
            x: -16
            style: TachGaugeStyle {}
            width: 833
            height: 50
            anchors.topMargin: 0
            minimumValue: 0
            maximumValue: 9000
            tickmarkStepSize: 75
            value: 0
            orientation: Qt.Horizontal
            anchors.top: parent.top

            Behavior on value {
                NumberAnimation {
                    duration: 5000
                }
            }
        }

        Text {
            id: txtRPM
            x: 702
            y: 78
            width: 98
            height: 46
            color: CF.getTachColor(tachGauge.value)
            text: Math.ceil(tachGauge.value) + '<font size="1"> RPM</font>'
            styleColor: "#000"
            style: Text.Normal
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignTop
            font.pixelSize: 36
        }

/****************** FUEL GAUGE ******************/

        Gauge {
            id: fuelGauge
            x: 0
            y: 441
            width: 185
            style: FuelGaugeStyle {}
            height: 7
            anchors.verticalCenterOffset: 220
            minimumValue: 0
            maximumValue: 100
            tickmarkStepSize: 25
            value: 0
            orientation: Qt.Horizontal
            anchors.verticalCenter: parent.verticalCenter

            Behavior on value {
                NumberAnimation {
                    duration: 4000
                }
            }
        }

        Rectangle {
            id: rectFuel
            x: fuelGauge.x + 13
            y: fuelGauge.y + 11
            width: Math.ceil(fuelGauge.width) / 4 - 25
            height: fuelGauge.height
            color: "#ff311a" // red
        }

        Image {
            id: imgFuel
            x: fuelGauge.x + (width / 2)
            y: fuelGauge.y - height - 4
            width: 28
            height: 25
            source: "../images/fuel_icon.png"
        }

        Text {
            id: txtFuel
            x: 115
            y: 424
            width: 76
            height: 32
            color: fuelGauge.value <= 20 ? "#ff0000" : "#fff"
            text: Math.ceil(fuelGauge.value) + '<font size="1">%</font>'
            style: Text.Normal
            verticalAlignment: Text.AlignTop
            styleColor: "#000000"
            horizontalAlignment: Text.AlignRight
            font.pixelSize: 28
        }

/****************** COOLANT TEMP GAUGE ******************/

        Gauge {
            id: coolantTempGauge
            x: 607
            y: 420
            width: 185
            style: TempGaugeStyle {}
            height: 7
            anchors.verticalCenterOffset: 153
            minimumValue: 50
            maximumValue: 140
            tickmarkStepSize: (maximumValue - minimumValue) / 4
            value: 0
            orientation: Qt.Horizontal
            anchors.verticalCenter: parent.verticalCenter

            Behavior on value {
                NumberAnimation {
                    duration: 4000
                }
            }
        }

        Rectangle {
            id: rectCoolantTempLow
            x: coolantTempGauge.x + 13
            y: coolantTempGauge.y + 11
            width: Math.ceil(coolantTempGauge.width) / 4 - 25
            height: coolantTempGauge.height
            color: "#4286f4" // blue
        }

        Rectangle {
            id: rectCoolantTempHigh
            x: rectCoolantTempLow.x + coolantTempGauge.width - (width * 2) - 6
            y: rectCoolantTempLow.y
            width: Math.ceil(coolantTempGauge.width) / 4 - 25
            height: coolantTempGauge.height
            color: "#ff311a" // red
        }

        Image {
            id: imgCoolantTemp
            x: coolantTempGauge.x + (width / 2)
            y: coolantTempGauge.y - height - 4
            width: 28
            height: 28
            source: "../images/coolant_temp_icon.png"
        }

        Text {
            id: txtCoolantTemp
            x: 692
            y: 357
            width: 98
            height: 46
            color: CF.getCoolantTempColor(coolantTempGauge.value)
            text: Math.ceil(coolantTempGauge.value) + '<font size="1">C</font>'
            horizontalAlignment: Text.AlignRight
            style: Text.Normal
            styleColor: "#000000"
            font.pixelSize: 28
            verticalAlignment: Text.AlignTop
        }

/****************** OIL TEMP GAUGE ******************/

        Gauge {
            id: oilTempGauge
            x: 605
            y: 200
            width: 185
            style: TempGaugeStyle {}
            height: 7
            anchors.verticalCenterOffset: 220
            minimumValue: 50
            maximumValue: 150
            tickmarkStepSize: (maximumValue - minimumValue) / 4
            value: 0
            orientation: Qt.Horizontal
            anchors.verticalCenter: parent.verticalCenter

            Behavior on value {
                NumberAnimation {
                    duration: 4000
                }
            }
        }

        Rectangle {
            id: rectOilTempLow
            x: oilTempGauge.x + 13
            y: oilTempGauge.y + 11
            width: Math.ceil(oilTempGauge.width) / 4 - 25
            height: oilTempGauge.height
            color: "#4286f4" // blue
        }

        Rectangle {
            id: rectOilTempHigh
            x: rectOilTempLow.x + oilTempGauge.width - (width * 2) - 6
            y: rectOilTempLow.y
            width: Math.ceil(oilTempGauge.width) / 4 - 25
            height: oilTempGauge.height
            color: "#ff311a" // red
        }

        Image {
            id: imgOilTemp
            x: oilTempGauge.x + (width / 2)
            y: oilTempGauge.y - height + 4
            width: 28
            height: 28
            source: "../images/oil_temp_icon.png"
        }

        Text {
            id: txtOilTemp
            x: 692
            y: 424
            width: 98
            height: 46
            color: CF.getCoolantTempColor(oilTempGauge.value)
            text: Math.ceil(oilTempGauge.value) + '<font size="1">C</font>'
            horizontalAlignment: Text.AlignRight
            style: Text.Normal
            styleColor: "#000000"
            font.pixelSize: 28
            verticalAlignment: Text.AlignTop
        }

/****************** VEHICLE SPEED ******************/

        Text {
            id: txtSpeed
            x: 400
            y: 304
            width: 98
            height: 46
            color: "#fff"
            text: '<b>68</b>' + '<font size="1"> MPH</font>'
            font.family: "Arial"
            horizontalAlignment: Text.AlignRight
            style: Text.Normal
            styleColor: "#000000"
            font.pixelSize: 46
            verticalAlignment: Text.AlignTop
        }

/****************** LAP TIMING ******************/

        Text {
            id: txtCurLap
            x: 264
            y: 223
            width: 398
            height: 58
            color: "#fff"
            text: "Current " + '<font size="4"> 0:47.10</font>'
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            font.pixelSize: 36
        }
    }
}
