import QtQuick 2.8
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import com.harman.game 1.0

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
        id: game_model
        rows: 6
        columns: 10
        onTargetNumberChanged:
        {
            targetLabel.text = targetNumber.toString()
        }
    }
    property alias view: gameField.gridView

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
            gameModel: game_model
        }

        Label
        {
            id: targetLabel
            color: "blue"
            font
            {
                family: "Arial"
                pointSize: 20
                bold: true
            }
            text: game_model.targetNumber.toString()
        }

        ControlPanel
        {
            Layout.columnSpan: 2
        }
    }

}
