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

    GameModel { id: gameModel }

    Component{ id: emptyComponent; Item {} }

    GameLogic
    {
        id: gameLogic
        model: gameModel.model // model pointer

        onModelChanged:
        {
            generateTargetNumber()
        }
        onTargetNumberChanged:
        {
            targetLabel.text = targetNumber.toString()
        }
        onGameStarted:
        {
            gameField.gridViewEnabled = true
            root.title = qsTr("Game Find Sum: running")
            animationLoader.sourceComponent = emptyComponent
        }
        onGameStopped:
        {
            gameField.gridViewEnabled = false
            root.title = qsTr("Game Find Sum: stopped")
            animationLoader.sourceComponent = emptyComponent
        }
        onGamePaused:
        {
            gameField.gridViewEnabled = false
            root.title = qsTr("Game Find Sum: paused")
        }
        onGameOver:
        {
            root.title = qsTr("Game Find Sum: GAME OVER !")
            animationLoader.sourceComponent = gameField.animationGameOver
        }
        onGameWin:
        {
            root.title = qsTr("Game Find Sum: GAME WIN !")
            controlPanel.startGameButtonEnabled = true
            animationLoader.sourceComponent = gameField.animationGameWin
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

            Loader
            {
                id: animationLoader
                anchors.centerIn: parent
            }
        }

        ColumnLayout
        {
            spacing: 2

            Label
            {
                id: targetLabel
                Layout.alignment: Qt.AlignCenter
                Layout.preferredHeight: gameField.height / 2

                color: "blue"
                font
                {
                    family: "Arial"
                    pointSize: 20
                    bold: true
                }
                text: "0"
            }

            LevelButton{ nLevel: 1 }
            LevelButton{ nLevel: 2 }
            LevelButton{ nLevel: 3 }
            LevelButton{ nLevel: 4 }
            LevelButton{ nLevel: 5 }
        }


        ControlPanel
        {
            id: controlPanel
            Layout.columnSpan: 2
        }
    }

}
