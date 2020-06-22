import QtQuick 2.7
import QtQuick.Controls 1.4

Rectangle
{
    id: root
    property int nButtonIndex
    property alias buttonText: text.text
    property alias buttonFont: text.font.pointSize
    signal clicked

    visible: false
    border.width: 1
    color: "white"

    MouseArea
    {
        anchors.fill: parent

        onPressed:
        {
            gameLogic.onUserAction(nButtonIndex, buttonText)
        }

        onClicked: root.clicked()
    }

    Text
    {
        id: text
        anchors.centerIn: parent
        font
        {
            family: "Arial"
            pointSize: 14
        }
        text: parent.buttonText
    }

}
