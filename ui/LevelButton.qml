import QtQuick 2.7
import QtQuick.Controls 1.4

Rectangle
{
    id: root
    property int nLevel
    signal clicked

    height: 30
    width: 60
    color: "lightblue"

    function actualLevel(nLevel)
    {
        levelLabel.text = "LEVEL: " + nLevel
    }

    MouseArea
    {
        anchors.fill: parent

        property color oldColor

        onPressed:
        {
            oldColor = root.color
            root.color = Qt.darker(oldColor)
            gameLogic.gameLevel = nLevel
            actualLevel(nLevel)
        }

        onReleased: root.color = oldColor

        onClicked: root.clicked()
    }

    Text
    {
        id: text
        anchors.centerIn: parent
        font
        {
            family: "Arial"
            italic: true
            pointSize: 12
        }
        text: "level " + nLevel.toString()
    }

}
