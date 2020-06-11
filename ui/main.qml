import QtQuick 2.8
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import com.harman.game_model 1.0
import com.harman.game_logic 1.0

Window
{
    id: root
    visible: true
    title: qsTr("Game Find Sum")

    property int margin: 11
    width: mainLayout.implicitWidth + 2 * margin
    height: mainLayout.implicitHeight + 2 * margin
    minimumWidth: mainLayout.Layout.minimumWidth + 2 * margin
    minimumHeight: mainLayout.Layout.minimumHeight + 2 * margin

    GameModel
    {
        id: gameModel
    }

    GameLogic
    {
        id: gameLogic
        model: gameModel.model

        onTargetNumberChanged:
        {
            targetLabel.text = targetNumber.toString()
        }
        onGameStarted:
        {
            gameField.gridViewEnabled = true
            root.title = qsTr("Game Find Sum: running")
        }
        onGameStopped:
        {
            gameField.gridViewEnabled = false
            root.title = qsTr("Game Find Sum: stopped")
        }
        onGamePaused:
        {
            gameField.gridViewEnabled = false
            root.title = qsTr("Game Find Sum: paused")
        }
        onGameOver:
        {
            root.title = qsTr("Game Find Sum: GAME OVER !")
        }
        onGameWin:
        {
            root.title = qsTr("Game Find Sum: GAME WIN !")
            controlPanel.startGameButtonEnabled = true
        }
    }

    GridLayout
    {
        id: mainLayout
        rows: 2
        columns: 2
        flow: GridLayout.LeftToRight
        anchors.fill: parent
        anchors.margins: margin

        GameField
        {
            id: gameField
            gridModel: gameModel
        }

        Label
        {
            id: targetLabel
            width: 30
            color: "blue"
            font
            {
                family: "Arial"
                pointSize: 20
                bold: true
            }
            text: "0"
        }

        ControlPanel
        {
            id: controlPanel
            Layout.columnSpan: 2
        }
    }

}
