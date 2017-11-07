import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2

Dialog {
    id: dialog
    title: qsTr("Procurar controlador")
    width: window.width < 300? window.width : 300
    height: 300

    modal: true
    x: (window.width - width) /2
    y: 50
    clip: true

    standardButtons: Dialog.Ok | Dialog.Cancel

    contentItem: Item {
        width: parent.width
        height: parent.height

        ColumnLayout {
            id: column
            anchors.fill: parent
            anchors.margins: 5
            spacing: 10

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: 40

                RoundButton {
                    text: "\uE8B6"
                    font.family: material_icon.name
                    font.pixelSize: 15
                    Material.background: Material.color(Material.Green, Material.Shade500)

                    anchors.right: parent.right
                    onClicked: {
                        networkDiscovery.clearController({});
                        networkDiscovery.sendRequestController();
                    }
                }
            }

            ListView {
                id: list_controller
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true

                ScrollIndicator.vertical: ScrollIndicator {}

                currentIndex: 0
                model: networkDiscovery.controller
                delegate: ItemDelegate {
                    width: parent.width
                    height: label_height.contentHeight + 20

                    Label {
                        id: label_height
                        text: modelData.text

                        anchors.fill: parent
                        anchors.margins: 5
                        wrapMode: Label.WordWrap
                        verticalAlignment: Label.AlignVCenter
                    }

                    highlighted: ListView.isCurrentItem
                    onClicked: list_controller.currentIndex = index
                }
            }
        }
    }

    onAccepted: {
        if (networkDiscovery.controller.length > 0) {
            settings.controller_name = networkDiscovery.controller[list_controller.currentIndex].device;
            settings.controller_addr = networkDiscovery.controller[list_controller.currentIndex].address;
        }
    }
}
