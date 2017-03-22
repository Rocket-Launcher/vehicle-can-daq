import QtQuick 2.0
import QtQuick.Controls.Styles 1.4
import "ColorFormatter.js" as CF

GaugeStyle {
    valueBar: Rectangle {
        implicitWidth: 75
        color: CF.getTachColor(tachGauge.value)
    }

    background: Rectangle {
        implicitWidth: 75
        color: "#3e3e3e"
    }

    minorTickmark: null
    tickmarkLabel: null
    foreground: null

    tickmark: Item {
        implicitWidth: 75
        implicitHeight: 2

        Rectangle {
            x: control.tickmarkAlignment === Qt.AlignLeft|| control.tickmarkAlignment === Qt.AlignTop ? parent.implicitWidth : parent.implicitWidth * -1
            width: parent.implicitWidth
            height: parent.implicitHeight
            color: "#000"
        }
    }
}
