import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

GroupBox
{
    Layout.fillWidth: true

    GridLayout
    {
        id: grid
        rows: 1
        flow: GridLayout.LeftToRight
        anchors.fill: parent

        ControlButton
        {
            id: startGame
            imageSource: "icons/start.png"
            Layout.fillWidth: true
            onClicked:
            {
                gameLogic.startGame()
            }
        }
        ControlButton
        {
            id: stopGame
            imageSource: "icons/stop.png"
            Layout.fillWidth: true
            onClicked:
            {
                gameLogic.stopGame()
            }
        }
        ControlButton
        {
            id: pauseGame
            imageSource: "icons/pause.png"
            Layout.fillWidth: true
            onClicked:
            {
                gameLogic.pauseGame()
            }

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
