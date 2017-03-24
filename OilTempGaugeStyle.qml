import QtQuick 2.0
import QtQuick.Controls.Styles 1.4
import "ColorFormatter.js" as CF

GaugeStyle {
    valueBar: Rectangle {
        implicitWidth: 7
        color: CF.getOilTempColor(oilTempGauge.value)
    }

    background: Rectangle {
        implicitWidth: 7
        color: "#3e3e3e"
    }

    foreground: null
    minorTickmark: null
    tickmarkLabel: null

    tickmark: Item {
        implicitWidth: 7
        implicitHeight: 4

        Rectangle {
            x: control.tickmarkAlignment === Qt.AlignLeft|| control.tickmarkAlignment === Qt.AlignTop ? parent.implicitWidth : parent.implicitWidth * -1
            width: parent.implicitWidth
            height: parent.implicitHeight
            color: "#000"
        }
    }
}
