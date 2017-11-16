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

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true

            TimerComponent {
                id: running_timer
                anchors.fill: parent

                MouseArea {
                    anchors.fill: parent
                }
            }
        }
    }
}
