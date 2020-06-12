import QtQuick 2.7
import QtQuick.Controls 1.4

Rectangle
{
    id: root
    property int nButtonValue
    property int nButtonIndex

    property alias bButtonVisible: root.visible
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
        property color buttonColor

        onPressed:
        {
            buttonColor = Qt.lighter("blue")
            gameLogic.onUserAction(nButtonValue, nButtonIndex, buttonColor)
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
        text: parent.text
    }

}
