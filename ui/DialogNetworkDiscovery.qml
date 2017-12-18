import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2

Dialog {
    id: dialog
    title: qsTr("Procurar controlador")
    width: window.width < 300? window.width -10 : 300
    height: 300

    modal: true
    x: (window.width - width) /2
    y: 50
    clip: true

    standardButtons: Dialog.Ok | Dialog.Cancel

    contentItem: Item {
        width: parent.width
        height: parent.height

        Column {
            anchors.fill: parent

            SwitchDelegate {
                width: parent.width
                height: 40
                text: qsTr("Configuração manual")
                checked: settings.controller_manual
                onCheckedChanged: settings.controller_manual = checked
            }

            RowLayout {
                visible: settings.controller_manual
                width: parent.width
                height: 60

                TextField {
                    id: textfield_controller_addr
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    text: settings.controller_addr
                    placeholderText: qsTr("Endereço de IP (ex: 192.168.1.10)")
                    validator: RegExpValidator {
                        regExp: /[0-9-.]+/
                    }
                }
            }

            RowLayout {
                visible: !settings.controller_manual
                width: parent.width
                height: 60

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
                visible: !settings.controller_manual
                width: parent.width
                height: parent.height -100
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
        if (networkDiscovery.controller.length > 0 && !settings.controller_manual) {
            settings.controller_name = networkDiscovery.controller[list_controller.currentIndex].device;
            settings.controller_addr = networkDiscovery.controller[list_controller.currentIndex].address;
        } else {
            settings.controller_addr = textfield_controller_addr.text
            settings.controller_name = ""
        }
    }
}
