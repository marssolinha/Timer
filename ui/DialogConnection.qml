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
                onCurrentIndexChanged: settings.type = currentIndex;
                Component.onCompleted: device_type.currentIndex = settings.type;
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
                    }

                    RowLayout {
                        width: parent.width
                        Label {
                            text: qsTr("Controlador:")
                            Layout.preferredWidth: 105
                            font.bold: true
                        }
                        ItemDelegate {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 35

                            Label {
                                anchors.fill: parent

                                text: (settings.controller_addr === "")? qsTr("Nenhum controlador definido") :
                                          (settings.controller_name)? settings.controller_name + " ("+ settings.controller_addr +")" : settings.controller_addr
                                color: Material.foreground
                                elide: Label.ElideRight

                                verticalAlignment: Label.AlignVCenter
                            }

                            onClicked: dialogNetworkDiscovery.open()
                        }
                    }

                    RowLayout {
                        width: parent.width
                        Label {
                            text: qsTr("Código PIN")
                            Layout.preferredWidth: 105
                            font.bold: true
                        }

                        ItemDelegate {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 35

                            TextField {
                                id: field_pin
                                width: parent.width
                                placeholderText: qsTr("Código PIN")
                            }
                        }
                    }

                    Button {
                        text: qsTr("Conectar")
                        width: parent.width
                        Material.background: Material.color(Material.Green, Material.Shade500)

                        onClicked: validateFields();
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
                            text: qsTr("Node dispositivo:")
                            Layout.preferredWidth: 105
                            font.bold: true
                        }
                        ItemDelegate {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 35

                            Label {
                                anchors.fill: parent
                                text: settings.local_name
                                horizontalAlignment: Label.AlignHCenter
                                verticalAlignment: Label.AlignVCenter
                                Layout.fillWidth: true
                                font.pixelSize: 24
                            }

                            onClicked: dialogDeviceName.open();
                        }
                    }

                    RowLayout {
                        width: parent.width
                        Label {
                            text: qsTr("PIN:");
                            Layout.preferredWidth: 105
                            font.bold: true
                        }
                        ItemDelegate {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 35

                            Label {
                                anchors.fill: parent
                                text: host.getPin();
                                horizontalAlignment: Label.AlignHCenter
                                verticalAlignment: Label.AlignVCenter
                                Layout.fillWidth: true
                                font.pixelSize: 24
                            }

                            //onClicked: dialogDeviceName.open();
                        }
                    }

                    RowLayout {
                        width: parent.width
                        Label {
                            text: qsTr("Endereço:");
                            Layout.preferredWidth: 105
                            font.bold: true
                        }
                        ItemDelegate {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 35

                            Label {
                                anchors.fill: parent
                                text: host.getAddress();
                                horizontalAlignment: Label.AlignHCenter
                                verticalAlignment: Label.AlignVCenter
                                Layout.fillWidth: true
                                font.pixelSize: 24
                            }

                            //onClicked: dialogDeviceName.open();
                        }
                    }
                }
            }
        }
    }

    function validateFields()
    {
        var validate = 0;
        var _message = "";

        if (settings.controller_addr === "") {
            ++validate;
            _message += "* É necessário indicar um controlador<br/>";
        }
        if (settings.controller_pin === "") {
            ++validate;
            _message += "* É necessário indicar o código PIN do controlador";
        }

        if (validate > 0) {
            message._text = _message;
            message.visible = true;
            return;
        }

    }
}
