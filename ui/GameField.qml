import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

GroupBox
{
    property alias gridModel: view.model
    property alias gridViewEnabled: view.enabled

    property alias nCellWidth: view.cellWidth
    property alias nCellHeight: view.cellHeight

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
            id: fieldButton

            nButtonIndex: itemIndex
            buttonText: itemValue.toString()
            visible: itemVisible
            color: itemColor

            width: nCellWidth - 5
            height: nCellHeight - 5
        }
    }

    GridView
    {
        id: view
        flow: GridView.FlowLeftToRight

        width: model.columns * nCellWidth
        height: model.rows * nCellHeight

        delegate: buttonDelegate

        Component
        {
            id: animationGameWin
            AnimImage
            {
                //width: nCellWidth * 5
                //height: nCellHeight * 3
                imageSource: "icons/game_win.png"
            }
        }
        Component
        {
            id: animationGameOver
            AnimImage
            {
                //width: nCellWidth * 5
                //height: nCellHeight * 3
                imageSource: "icons/game_over.png"
            }
        }

    } // GridView

} // GroupBox
