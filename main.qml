import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Timer")

    header: ToolBar {
        width: parent.width
        height: 50

        Material.background: Material.color(Material.DeepPurple, Material.Shade500)

        RowLayout {
            anchors.fill: parent

            Text {
                text: "\uE855"
                font.pixelSize: 32
                color: "#FFF"
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                Layout.topMargin: 5
            }

            Text {
                text: qsTr("Timer")
                color: "#FFF"
                font.pixelSize: 24
                Layout.fillWidth: true
                Layout.topMargin: 0
            }

            RowLayout {
                Layout.fillHeight: true

                ToolButton {
                    Layout.fillHeight: true
                    text: "\uE0C3"
                    font.pixelSize: 28
                }
                ToolButton {
                    Layout.fillHeight: true
                    text: "\uE8B8"
                    font.pixelSize: 28
                }
            }
        }
    }

    Page {
        anchors.fill: parent

        Rectangle {
            anchors.fill: parent
            color: "transparent"

            TimerComponent {
                width: parent.width
                height: parent.height /2
            }
        }
    }

    FontLoader {
        id: material_icon
        source: "qrc:/fonts/MaterialIcons.ttf"
    }
}
