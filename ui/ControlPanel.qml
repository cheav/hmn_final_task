import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

GroupBox
{
    Layout.fillWidth: true

    GridLayout
    {
        rows: 1
        flow: GridLayout.LeftToRight
        anchors.fill: parent

        ControlButton
        {
            id: startGame
            imageSource: "images/start.png"
            Layout.fillWidth: true
            //onClicked: gameModel
        }
        ControlButton
        {
            id: stopGame
            imageSource: "images/stop.png"
            Layout.fillWidth: true
        }
        ControlButton
        {
            id: pauseGame
            imageSource: "images/pause.png"
            Layout.fillWidth: true
        }
        ControlButton
        {
            text: "Quit"
            textColor: "green"
            Layout.fillWidth: true
            onClicked: Qt.quit()
        }
    }
}
