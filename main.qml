import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2

import StatusBar 0.1
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
                    text: "\uE0C3"
                    font.family: material_icon.name
                    font.pixelSize: 28
                }
                ToolButton {
                    Layout.fillHeight: true
                    text: "\uE8B8"
                    font.family: material_icon.name
                    font.pixelSize: 28
                }
            }
        }
    }

    StackView {
        anchors.fill: parent

        initialItem: Item {
            anchors.fill: parent

            ColumnLayout {
                anchors.fill: parent
                spacing: 2

                Item {
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillHeight: !object.timer_started
                    Layout.fillWidth: !object.timer_started
                    width: 150
                    height: 75

                    TimerComponent {
                        id: timer_prepare
                        anchors.fill: parent
                    }
                }

                Item {
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillHeight: object.timer_started
                    Layout.fillWidth: object.timer_started
                    width: parent.width
                    height: 75

                    TimerComponent {
                        visible: object.timer_started
                        id: counttimer
                        anchors.fill: parent
                    }
                }

                Item {
                    Layout.alignment: Qt.AlignCenter
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: 100

                    RoundButton {
                        width: 90
                        height: width

                        anchors.centerIn: parent
                        Material.background: Material.color((object.timer_started? Material.Red : Material.Green), Material.Shade500)
                        text: object.timer_started? "\uE047" : "\uE037"
                        font.family: material_icon.name
                        font.pixelSize: Math.round(parent.height * 0.65)

                        onClicked: object.timer_started? action("stop") : action("start");
                    }
                }
            }
        }
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

    FontLoader {
        id: material_icon
        source: "qrc:/fonts/MaterialIcons.ttf"
    }

    StatusBar {
        color: Material.background
    }
}
