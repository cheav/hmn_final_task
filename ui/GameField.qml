import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

GroupBox
{
    property alias gridModel: view.model
    property alias gridView: view
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

        property int nButtonSize: 60
        property int nButtonWidth: nButtonSize
        property int nButtonHeight: nButtonSize

        width: model.columns * nButtonWidth
        height: model.rows * nButtonHeight
        cellWidth: nButtonWidth
        cellHeight: nButtonHeight

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
