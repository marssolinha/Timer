import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2

Dialog {
    id: dialog
    title: qsTr("Dispositivos conectados")
    width: window.width < 300? window.width : 300
    height: 200

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
            spacing: 10

            ListView {
                visible: tcp_connect.devices > 0
                Layout.fillHeight: true
                Layout.fillWidth: true
                clip: true

                model: tcp_connect.list_devices
                delegate: ItemDelegate {
                    width: parent.width
                    height: 35
                    text: modelData.ip

                    onPressAndHold: tcp_connect.server_disconnectClient(index);
                }
            }

            Label {
                visible: tcp_connect.devices === 0
                text: qsTr("Nenhum dispositivo conectado")
                horizontalAlignment: Label.AlignHCenter
                Layout.fillWidth: true
            }
        }
    }
}
