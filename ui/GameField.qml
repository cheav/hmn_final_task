import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

GroupBox
{
    property alias gridModel: view.model
    property alias gridViewEnabled: view.enabled

    property alias animationGameWin: animationGameWin
    property alias animationGameOver: animationGameOver

    id: gridBox
    Layout.fillWidth: true
    Layout.fillHeight: true

    Component
    {
        id: buttonDelegate

        FieldButton
        {
            nButtonIndex: itemIndex
            buttonText: itemValue.toString()
            visible: itemVisible
            color: itemColor
        }
    }

    GridView
    {
        id: view
        flow: GridView.FlowLeftToRight

        property int nCellSize: 60

        width: model.columns * nCellSize
        height: model.rows * nCellSize
        cellWidth: nCellSize
        cellHeight: nCellSize

        delegate: buttonDelegate

        Component
        {
            id: animationGameWin
            GameImage
            {
                imageSource: "icons/game_win.png"
            }
        }
        Component
        {
            id: animationGameOver
            GameImage
            {
                imageSource: "icons/game_over.png"
            }
        }

    } // GridView

} // GroupBox
