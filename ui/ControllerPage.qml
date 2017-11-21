import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2

Item {
    id: item
    property alias running_timer: running_timer

    anchors.fill: parent

    ColumnLayout {
        anchors.fill: parent;
        spacing: 0

        Rectangle {
            id: rectangle_timer
            //color: Material.color(Material.DeepOrange, Material.Shade500)
            //color: object.alert? object.alert_color : "transparent";
            color: "transparent"
            Layout.fillHeight: true
            Layout.fillWidth: true

            TimerComponent {
                id: define_timer
                visible: !countdown.status_timer
                anchors.fill: parent
            }

            TimerComponent {
                id: running_timer
                visible: countdown.status_timer
                anchors.fill: parent

                MouseArea {
                    anchors.fill: parent
                }
            }

            SequentialAnimation on color {
                running: object.alert
                ColorAnimation { to: object.alert_color; duration: (settings.controller_alert * 1000 * 0.6) }
            }

            SequentialAnimation on color {
                running: !object.alert
                ColorAnimation { to: "transparent"; duration: 5000 }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 1
            color: object.border_up
        }
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: 3
        }
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 1
            color: object.border_down
        }

        RowLayout {
            Layout.fillWidth: true

            Item {
                Layout.preferredHeight: 100
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

                    onClicked: {
                        if (object.timer_paused) {
                            countdown.prepareResumeTime();
                        } else if (!object.timer_started) {
                            countdown.setTimeString(completeZero(define_timer.getHours) + ":" +
                                                    completeZero(define_timer.getMinutes) + ":" +
                                                    completeZero(define_timer.getSeconds));
                        } else {
                            countdown.preparePauseTime();
                        }
                    }
                }
            }

            Item {
                Layout.preferredHeight: 100
                Layout.preferredWidth: 50

                ToolButton {
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
}
