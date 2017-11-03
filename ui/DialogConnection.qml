import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2


Dialog {
    id: dialog
    title: qsTr("Conexão")
    width: window.width < 400? window.width : 400
    height: 400

    modal: true
    x: (window.width - width) /2
    padding: 1
    clip: true

    contentItem: Flickable {
        width: parent.width
        height: parent.height
        contentHeight: parent.height *1.1

        Column {
            id: column
            anchors.fill: parent
            anchors.margins: 5
            spacing: 10

            Label {
                width: parent.width
                text: qsTr("Definir este dispositivo como: ")
                wrapMode: Label.WrapAnywhere
                font.bold: true
            }

            ListModel {
                id: list_type_device
                ListElement {type: "Receptor"}
                ListElement {type: "Controlador"}
            }

            ComboBox {
                id: device_type
                width: parent.width
                model: list_type_device

                //list_type_device.get(device_type.currentIndex)['type']
                onCurrentIndexChanged: {

                }
            }

            GroupBox {
                id: groupbox_receptor
                visible: device_type.currentIndex == 0
                width: parent.width

                Column {
                    width: parent.width
                    spacing: 2

                    RowLayout {
                        width: parent.width

                        Label {
                            text: qsTr("Dados do controlador")
                            font.bold: true
                            Layout.fillWidth: true
                        }

                        RoundButton {
                            text: "\uE8B6"
                            font.family: material_icon.name
                            font.pixelSize: 15
                            Material.background: Material.color(Material.Green, Material.Shade500)

                            onClicked: {
                                console.log("Search");
                            }
                        }
                    }

                    ComboBox {
                        id: list_controler
                        width: parent.width
                    }

                    TextField {
                        id: field_pin
                        width: parent.width
                        placeholderText: qsTr("Código PIN")
                    }

                    Button {
                        text: qsTr("Conectar")
                        width: parent.width
                        Material.background: Material.color(Material.Green, Material.Shade500)
                    }
                }
            }

            GroupBox {
                id: groupbox_controlador
                visible: device_type.currentIndex == 1
                width: parent.width

                Column {
                    width: parent.width
                    spacing: 2

                    Label {
                        text: qsTr("Controlador")
                        font.bold: true
                    }

                    RowLayout {
                        width: parent.width
                        Label {
                            text: qsTr("Nome:");
                            width: 30
                            font.bold: true
                        }
                        Label {
                            text: host.getHostname();
                            horizontalAlignment: Label.AlignHCenter
                            Layout.fillWidth: true
                        }
                    }

                    RowLayout {
                        width: parent.width
                        Label {
                            text: qsTr("PIN:");
                            width: 30
                            font.bold: true
                        }
                        Label {
                            text: host.getPin();
                            horizontalAlignment: Label.AlignHCenter
                            Layout.fillWidth: true
                            font.pixelSize: 28
                        }
                    }

                    RowLayout {
                        width: parent.width
                        Label {
                            text: qsTr("Endereço:");
                            width: 30
                            font.bold: true
                        }
                        Label {
                            text: host.getAddress();
                            horizontalAlignment: Label.AlignHCenter
                            Layout.fillWidth: true
                            font.pixelSize: 28
                        }
                    }
                }
            }
        }
    }
}
