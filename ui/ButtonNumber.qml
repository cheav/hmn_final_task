import QtQuick 2.7
import QtQuick.Controls 1.4

Rectangle
{
    id: root
    property int nButtonNumber
    property int nButtonIndex

    property alias buttonVisible: root.visible
    property alias text: text.text
    property alias textColor: text.color
    signal clicked

    visible: false
    height: 50
    width: 50
    border.width: 1
    color: "white"

    MouseArea
    {
        anchors.fill: parent
        property color oldColor
        //
        onPressed:
        {
            oldColor = root.color
            //root.color = Qt.darker(oldColor)
            root.color = Qt.lighter("green")
            gameLogic.reactionOnUserAction(nButtonNumber, nButtonIndex)

            //(game_model.data(gridView.currentIndex)).toString()
        }
        //
        //onReleased: root.color = oldColor

        onClicked: root.clicked()
    } // MouseArea

    Text
    {
        id: text
        anchors.centerIn: parent
        font
        {
            family: "Arial"
            pointSize: 14
        }
        text: parent.text
    }

} // Rectangle
