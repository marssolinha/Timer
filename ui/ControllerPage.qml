import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2

Item {
    id: item
    property alias running_timer: running_timer
    property alias define_timer: define_timer

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
    }
}
