import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2

Item {
    id: item
    property alias running_timer: running_timer

    anchors.fill: parent

    ColumnLayout {
        anchors.fill: parent

        Rectangle {
            color: "transparent"
            Layout.fillHeight: true
            Layout.fillWidth: true

            TimerComponent {
                id: running_timer
                anchors.fill: parent

                MouseArea {
                    anchors.fill: parent
                }
            }

            SequentialAnimation on color {
                running: object.alert
                ColorAnimation { to: object.alert_color; duration: (settings.controller_alert * 1000 * 0.75) }
            }

            SequentialAnimation on color {
                running: !object.alert
                ColorAnimation { to: "transparent"; duration: 5000 }
            }
        }
    }
}
