import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

GroupBox
{
    property alias startGameButtonEnabled: startGame.enabled
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
                targetLabel.text = gameLogic.targetNumber.toString()
                gameLogic.startGame()
                enabled = false
                stopGame.enabled = true
                pauseGame.enabled = true
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
                targetLabel.text = "0"
                enabled = false
                startGame.enabled = true
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
                enabled = false
                startGame.enabled = true
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
