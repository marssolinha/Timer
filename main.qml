import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2
import Qt.labs.settings 1.0

import StatusBar 0.1
import HostInfo 0.1
import NetworkDiscovery 0.1
import TcpConnect 0.1
import Countdown 0.1

import "./ui"

ApplicationWindow {
    id: window
    visible: true
    width: 380
    height: 500
    title: qsTr("Timer")
    flags: Qt.Window | Qt.MaximizeUsingFullscreenGeometryHint
    font: ubuntu.name

    QtObject {
        id: object

        property bool timer_started: false
        property bool timer_paused: false
        property int _time: 0
        property int _counttimer: 0
        property bool alert: countdown.alert
        property color alert_color: Material.color(Material.Red, Material.Shade900)
        property color border_up: "#101010"
        property color border_down: "#2f2f2f"
    }

    Material.accent: Material.color(Material.Red, Material.Shade500)
    Material.background: "#222222"

    Settings {
        id: settings
        property int type: 0
        property int pin: 0
        property string controller_name: ""
        property string controller_addr: ""
        property string controller_pin: ""
        property int controller_alert: 0

        property string local_name: ""
        property string local_addr: ""

        Component.onCompleted: {
            if (pin == 0 || pin === "")
                pin = host.getPin();
        }

        onController_alertChanged: countdown.timeToString(controller_alert);
    }

    Shortcut {
        sequence: "Ctrl+C"
        onActivated: settings.type = 1
    }

    Shortcut {
        sequence: "Ctrl+R"
        onActivated: settings.type = 0
    }

    Shortcut {
        sequence: "Alt+C"
        onActivated: dialogConnection.validateFields()
    }

    Shortcut {
        sequence: "Ctrl+S"
        onActivated: !object.timer_started ? start_pause_timer() : ""
    }

    Shortcut {
        sequence: "Ctrl+P"
        onActivated: object.timer_started ? start_pause_timer() : ""
    }

    Shortcut {
        sequence: "Ctrl+B"
        onActivated: object.timer_started || object.timer_paused ? countdown.prepareStopTime() : ""
    }

    header: ToolBar {
        Material.primary: object.alert_color
        topPadding: Qt.platform.os === "ios" ? Screen.height - Screen.desktopAvailableHeight : 0

        RowLayout {
            anchors.fill: parent

            Label {
                text: "\uE924"
                font.family: material_icon.name
                font.pixelSize: 32
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                Layout.topMargin: 3
            }

            Label {
                text: qsTr("Timer")
                font.pixelSize: 24
                font.weight: Font.Light
                Layout.fillWidth: true
                Layout.topMargin: 0
            }

            RowLayout {
                Layout.fillHeight: true

                ToolButton {
                    visible: object.timer_started && settings.type == 1
                    Layout.fillHeight: true
                    text: "\uE308"
                    font.family: material_icon.name
                    font.pixelSize: 28

                    onClicked: dialogDevicesConnected.open()
                }

                ToolButton {
                    enabled: !object.timer_started
                    Layout.fillHeight: true
                    text: "\uE337"
                    font.family: material_icon.name
                    font.pixelSize: 28

                    Rectangle {
                        width: 16
                        height: width
                        radius: width
                        anchors.right: parent.right
                        anchors.rightMargin: 5
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 5
                        color: Qt.rgba(255, 255, 255, 0.80)

                        Text {
                            visible: settings.type
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            text: tcp_connect.devices
                            font.pixelSize: 12
                            font.bold: true
                            color: object.alert_color
                        }

                        Rectangle {
                            visible: !settings.type
                            width: 8
                            height: width
                            radius: width
                            anchors.centerIn: parent
                            color: tcp_connect.receiver_connect? Material.color(Material.Green, Material.Shade600) : object.alert_color
                        }
                    }

                    onClicked: {
                        dialogConnection.open();
                        statusBar.theme = StatusBar.Dark
                    }
                }
            }
        }
    }

    footer: ToolBar {
        enabled: settings.type === 1
        visible: settings.type === 1
        height: 100
        Material.background: Material.background

        RowLayout {
            anchors.fill: parent

            Item {
                Layout.fillHeight: true
                Layout.preferredWidth: 50

                ToolButton {
                    visible: object.timer_started
                    Layout.fillHeight: true
                    anchors.centerIn: parent
                    text: "\uE047"
                    font.family: material_icon.name
                    font.pixelSize: 45

                    onClicked: countdown.prepareStopTime()
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: 100

                RoundButton {
                    width: 90
                    height: width

                    anchors.centerIn: parent
                    Material.background: Material.color((object.timer_started? Material.Amber : Material.Green), Material.Shade500)
                    Material.elevation: 10
                    text: object.timer_started? "\uE034" : "\uE037"
                    font.family: material_icon.name
                    font.pixelSize: Math.round(parent.height * 0.65)

                    onClicked: start_pause_timer();
                }
            }

            Item {
                Layout.preferredHeight: 100
                Layout.preferredWidth: 50

                ToolButton {
                    enabled: !object.timer_started
                    Layout.fillHeight: true
                    anchors.centerIn: parent
                    text: "\uE8B8"
                    font.family: material_icon.name
                    font.pixelSize: 30

                    onClicked: dialogSettings.open()
                }
            }
        }
    }

    function start_pause_timer()
    {
        if (object.timer_paused) {
            countdown.prepareResumeTime();
        } else if (!object.timer_started) {
            countdown.setTimeString(completeZero(controllerPage.define_timer.getHours) + ":" +
                                    completeZero(controllerPage.define_timer.getMinutes) + ":" +
                                    completeZero(controllerPage.define_timer.getSeconds));
        } else {
            countdown.preparePauseTime();
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

    /*Rectangle {
        width: parent.width
        height: 25
        color: "transparent"

        Label {
            anchors.fill: parent
            horizontalAlignment: Label.AlignHCenter
            verticalAlignment: Label.AlignVCenter
            text: qsTr("Tempo real: ") + countdown.getRealTime
            font.pixelSize: 10
        }
    }*/

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

    DialogDevicesConnected {
        id: dialogDevicesConnected
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
        if (length === undefined)
            length = 2;
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

    HostInfo {
        id: host

        onPinChanged: settings.pin = getPin();
    }

    Countdown {
        id: countdown
        time_alert: settings.controller_alert

        onSecondsChanged: (settings.type == 0)? receiverPage.running_timer.setSeconds.currentIndex = seconds :
                                                controllerPage.running_timer.setSeconds.currentIndex = seconds
        onMinutesChanged: (settings.type == 0)? receiverPage.running_timer.setMinutes.currentIndex = minutes :
                                                controllerPage.running_timer.setMinutes.currentIndex = minutes
        onHoursChanged: (settings.type == 0)? controllerPage.running_timer.setMinutes.currentIndex = minutes :
                                              controllerPage.running_timer.setMinutes.currentIndex = minutes
        onStatus_timerChanged: object.timer_started = status_timer
        onTimerPauseChanged: object.timer_paused = timerPause
        onSend_timerChanged: tcp_connect.setData_send(send_timer)
        onSend_commandChanged: tcp_connect.setData_send(send_command)
        onSend_timerIfRunningChanged: tcp_connect.setData_sendIfRunning(send_timerIfRunning)
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
        }
    }

    NetworkDiscovery {
        id: networkDiscovery
        type: settings.type
        device: settings.local_name

        onConnect_controllerChanged: tcp_connect.connectToController(connect_controller);
    }

    TcpConnect {
        id: tcp_connect
        serviceType: settings.type
        local_addr: settings.local_addr
        addressController: settings.controller_addr
        pinPass: settings.pin
        pinController: settings.controller_pin
        onReceive_current_time_controllerChanged: countdown.getCurrentTimeController(receive_current_time_controller)
        onReceive_timerChanged: countdown.setTimeFromController(receive_timer)
        onReceive_commandChanged: countdown.getCommand(receive_command)
        onAddressControllerChanged: settings.controller_addr = addressController
        onNameControllerChanged: settings.controller_name = nameController
        onSend_TimerIfRunning: countdown.prepareRequestTimerIfRunning()
    }

    Connections {
        target: settings
    }

    StatusBar {
        id: statusBar
        theme: Material.Dark
        color: object.alert_color
    }

    FontLoader {
        id: ubuntu
        source: "qrc:/fonts/Ubuntu.ttf"
    }

    FontLoader {
        id: material_icon
        source: "qrc:/fonts/MaterialIcons.ttf"
    }

    onClosing: {
        if (Qt.platform.os == "android")
        {
            close.accepted = false
        }
    }
}
