import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2

Item {
    id: item
    anchors.fill: parent

    ColumnLayout {
        anchors.fill: parent;

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true

            TimerComponent {
                id: define_timer
                anchors.fill: parent
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 1
            color: "#292929"
        }
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 1
            color: "#333"
        }

        RowLayout {
            Layout.fillWidth: true

            Item {
                Layout.preferredHeight: 100
                Layout.preferredWidth: 50
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: 100

                RoundButton {
                    width: 90
                    height: width

                    anchors.centerIn: parent
                    Material.background: Material.color((object.timer_started? Material.Red : Material.Green), Material.Shade500)
                    text: object.timer_started? "\uE047" : "\uE037"
                    font.family: material_icon.name
                    font.pixelSize: Math.round(parent.height * 0.65)

                    onClicked: {
                        countdown.setTime(completeZero(define_timer.getHours) + ":" +
                                          completeZero(define_timer.getMinutes) + ":" +
                                          completeZero(define_timer.getSeconds));
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
                    font.pixelSize: 24

                    onClicked: dialogSettings.open()
                }
            }
        }
    }
}
