import QtQuick 2.7

Rectangle
{
    id: root

    property alias text: text.text
    property alias imageSource: image.source
    property alias textColor: text.color
    signal clicked

    height: 32
    width: 32
    border.width: 0
    color: "white"
    //
    Image
    {
        id: image
        anchors.centerIn: parent
    }

    MouseArea
    {
        anchors.fill: parent
        //
        property color oldColor
        //
        onPressed:
        {
            oldColor = root.color
            root.color = Qt.darker(oldColor)
        }
        //
        onReleased: root.color = oldColor

        onClicked: root.clicked()
    }

    Text
    {
        id: text
        anchors.centerIn:parent
        font.pointSize: 12
        text: parent.text
    }

}
