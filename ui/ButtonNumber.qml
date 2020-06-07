import QtQuick 2.7
import QtQuick.Controls 1.4

Rectangle
{
    id: root
    property int nButtonNumber

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
            game_model.reactionOnUserAction(nButtonNumber, view.currentIndex)

//            if(game_model.equalityTest())
//            {
//                root.visible = false
//            }
            //strNumberSelected = (game_model.data(gridView.currentIndex)).toString()
            //game_model.setData(view.currentIndex, "green")
            //numberColor = root.color
        }
        //
        //onReleased: root.color = oldColor

        onClicked: root.clicked()
    } // MouseArea

    Text
    {
        id: text
        anchors.centerIn:parent
        //font.pointSize: 12
        font
        {
            family: "Arial"
            pointSize: 14
            //bold: true
        }
        text: parent.text
    }

} // Rectangle
