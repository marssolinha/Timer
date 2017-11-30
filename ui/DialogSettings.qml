import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2

Dialog {
    id: dialog
    property alias timer_alert: timer_alert

    title: qsTr("Configurações")
    width: window.width < 400? window.width -10 : 400
    height: 260

    modal: true
    x: (window.width - width) /2
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

            Label {
                Layout.fillWidth: true
                font.bold: true
                text: qsTr("Tempo de alerta")
            }

            TimerComponent {
                id: timer_alert

                itemWidth: parent.width
                itemHeight: 80
                textSize: height
            }

            Item {
                Layout.fillHeight: true
            }

            RowLayout {
                visible: settings.controller_alert > 0
                Layout.fillWidth: true

                Label {
                    text: qsTr("Alerta definido")
                    font.bold: true
                }

                Label {
                    text: countdown.convertHours + ":" + countdown.convertMinutes + ":" + countdown.convertSeconds
                }
            }
        }
    }
    onAccepted: {
        settings.controller_alert = new Date("1970-01-01 "+completeZero(timer_alert.getHours)+":"+completeZero(timer_alert.getMinutes)+":"+completeZero(timer_alert.getSeconds)+" -0000").getTime() /1000;
    }
}
