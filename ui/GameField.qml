import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

GroupBox
{
    property alias gameModel: view.model
    property alias gridView: view

    id: gridBox
    Layout.fillWidth: true
    Layout.fillHeight: true
    //
    Component
    {
        id: buttonDelegate

        ButtonNumber
        {
            nButtonNumber: numberValue
            buttonVisible: numberVisible
            text: numberValue.toString()
            //color: game_model.data(view.currentIndex) //numberColor
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
        cellWidth: width / model.columns
        cellHeight: height / model.rows

        delegate: buttonDelegate
    }

} // GroupBox
