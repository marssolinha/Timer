import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2

import StatusBar 0.1
import "./ui"

ApplicationWindow {
    visible: true
    width: 380
    height: 500
    title: qsTr("Timer")

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
                color: Material.color(Material.DeepPurple, Material.Shade300)
                font.family: material_icon.name
                font.pixelSize: 32
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                Layout.topMargin: 3
            }

            Label {
                text: qsTr("Timer")
                color: Material.color(Material.DeepPurple, Material.Shade300)
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

    StatusBar {
        color: Material.background
    }
}
