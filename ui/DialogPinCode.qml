import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2

Dialog {
    id: dialog
    title: qsTr("Código PIN")
    width: window.width < 300? window.width -10 : 300
    height: 250

    modal: true
    x: (window.width - width) /2
    y: 50
    padding: 1
    clip: true

    standardButtons: Dialog.Ok

    contentItem: Item {
        width: parent.width
        height: parent.height

        ColumnLayout {
            id: column
            anchors.fill: parent
            anchors.margins: 5

            RowLayout {
                Layout.fillWidth: true

                Label {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    verticalAlignment: Label.AlignVCenter
                    horizontalAlignment: Label.AlignHCenter
                    text: qsTr("Configuração manual")
                }

                Switch {
                    id: switch_pin
                    Layout.fillHeight: true
                    checked: settings.pin_manual
                    onCheckedChanged: settings.pin_manual = checked
                }
            }

            RowLayout {
                visible: !switch_pin.checked
                Layout.fillWidth: true
                Layout.fillHeight: true

                Label {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    text: settings.pin

                    horizontalAlignment: Label.AlignHCenter
                    verticalAlignment: Label.AlignVCenter
                    font.pixelSize: 24
                }

                RoundButton {
                    text: "\uE5D5"
                    font.family: material_icon.name
                    font.pixelSize: 15
                    Material.background: Material.color(Material.Green, Material.Shade500)

                    onClicked: host.generatePin(Date.now());
                }
            }

            RowLayout {
                visible: switch_pin.checked
                Layout.fillWidth: true

                Item {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 60

                    TextField {
                        width: parent.width * 0.7
                        anchors.centerIn: parent
                        placeholderText: qsTr("Código PIN")
                        text: settings.pin
                        onTextChanged: (text === "")? 0 : settings.pin = text
                        validator: IntValidator {
                            bottom: 1111
                            top: 9999
                        }
                    }
                }
            }
        }
    }
}
