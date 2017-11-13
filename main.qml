import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2
import Qt.labs.settings 1.0

import StatusBar 0.1
import HostInfo 0.1
import NetworkDiscovery 0.1
import TcpConnect 0.1

import "./ui"

ApplicationWindow {
    id: window
    visible: true
    width: 380
    height: 500
    title: qsTr("Timer")

    QtObject {
        id: object

        property bool timer_started: false
        property int _time: 0
        property int _counttimer: 0
    }

    Settings {
        id: settings
        property int type: 0
        property int pin: 0
        property string controller_name: ""
        property string controller_addr: ""
        property string controller_pin: ""

        property string local_name: ""
        property string local_addr: ""

        Component.onCompleted: {
            if (pin == 0 || pin === "")
                pin = host.getPin();
        }
    }

    header: Rectangle {
        width: parent.width
        height: 50

        color: Material.background

        Rectangle {
            width: parent.width
            height: 1
            color: "#292929"
            anchors.bottom: row_bottom.top
        }
        Rectangle {
            id: row_bottom
            width: parent.width
            height: 1
            color: "#333"
            anchors.bottom: parent.bottom
        }

        RowLayout {
            anchors.fill: parent

            Label {
                text: "\uE425"
                color: Material.color(Material.DeepOrange, Material.Shade500)
                font.family: material_icon.name
                font.pixelSize: 32
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                Layout.topMargin: 3
            }

            Label {
                text: qsTr("Timer")
                color: Material.color(Material.DeepOrange, Material.Shade500)
                font.pixelSize: 24
                font.weight: Font.Light
                Layout.fillWidth: true
                Layout.topMargin: 0
            }

            RowLayout {
                Layout.fillHeight: true

                ToolButton {
                    Layout.fillHeight: true
                    text: "\uE337"
                    font.family: material_icon.name
                    font.pixelSize: 28

                    onClicked: dialogConnection.open();
                }
            }
        }
    }

    ReceiverPage {
        id: receiverPage
        visible: settings.type == 0
    }

    ControllerPage {
        id: controllerPage
        visible: settings.type == 1
    }

    DialogConnection {
        id: dialogConnection
    }

    DialogDeviceName {
        id: dialogDeviceName
    }

    DialogPinCode {
        id: dialogPinCode
    }

    DialogNetworkDiscovery {
        id: dialogNetworkDiscovery
    }

    DialogSettings {
        id: dialogSettings
    }

    Timer {
        id: timer
        interval: 1000
        repeat: true
        onTriggered: {
            ++object._counttimer;
            var str_time = new Date(object._counttimer * 1000);
            counttimer.setHours.currentIndex = str_time.getUTCHours();
            counttimer.setMinutes.currentIndex = str_time.getUTCMinutes();
            counttimer.setSeconds.currentIndex = str_time.getUTCSeconds();

            //if (object._counttimer > object._time)
        }
    }

    function action(_action)
    {
        if (_action === "start") {
            object.timer_started = true;
            object._time = getTimeSeconds(
                        "1970-01-01 "
                        + completeZero(timer_prepare.getHours, 2) + ":"
                        + completeZero(timer_prepare.getMinutes, 2) + ":"
                        + completeZero(timer_prepare.getSeconds, 2)
                        );
            object._counttimer = 0;
            timer.running = true
        } else if (_action === "stop") {
            object.timer_started = false;
            object._counttimer = 0;
            object._time = 0;
        }
    }

    function getTimeSeconds(str)
    {
        var time = new Date(str + " -0000");
        return time.getTime() /1000;
    }

    function completeZero(str, length)
    {
        str = str.toString();
        while (str.length < length) {
            str = "0" + str;
        }
        return str;
    }

    ToolTip {
        id: message
        property string _text

        timeout: 5000
        topMargin: parent.height -tooltip_text.parent.height -50
        x: (window.width - tooltip_text.parent.width) /2 -20
        z: 100

        width: 0
        height: 0

        Rectangle {
            width: tooltip_text.contentWidth + 10
            height: tooltip_text.contentHeight + 10

            color: Qt.rgba(Material.background.r, Material.background.g, Material.background.b, 0.8)
            radius: 3

            Column {
                anchors.fill: parent
                padding: 5

                Text {
                    id: tooltip_text
                    width: window.width *0.8
                    height: contentHeight

                    anchors.margins: 5

                    text: message._text
                    color: Material.foreground

                    wrapMode: Text.WordWrap
                }
            }
        }
    }

    FontLoader {
        id: material_icon
        source: "qrc:/fonts/MaterialIcons.ttf"
    }

    HostInfo {
        id: host

        onPinChanged: settings.pin = getPin();
    }

    NetworkDiscovery {
        id: networkDiscovery
        type: settings.type
        device: settings.local_name

        //onControllerChanged: console.log(Object.keys(controller))
    }

    TcpConnect {
        id: tcp_connect
        serviceType: settings.type
        addressController: settings.controller_addr
    }

    Connections {
        target: settings
        //onTypeChanged: console.log("Type change", settings.type);
    }

    StatusBar {
        color: Material.background
    }
}
