import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2

Dialog {
    id: dialog
    title: qsTr("Código PIN")
    width: window.width < 300? window.width -10 : 300
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

            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: 40

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
        }
    }
}
