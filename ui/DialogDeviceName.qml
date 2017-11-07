import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2

Dialog {
    id: dialog
    title: qsTr("Nome do dispositivo")
    width: window.width < 300? window.width : 300
    height: 200

    modal: true
    x: (window.width - width) /2
    y: 50
    padding: 1
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

            TextField {
                id: textfield_controller_name
                Layout.fillWidth: true
                text: settings.local_name
            }
        }
    }

    onAccepted: settings.local_name = textfield_controller_name.text
}
