import QtQuick 2.8
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4

Window
{
    visible: true
    title: qsTr("Game Find Sum")

    property int margin: 11
    width: mainLayout.implicitWidth + 2 * margin
    height: mainLayout.implicitHeight + 2 * margin
    minimumWidth: mainLayout.Layout.minimumWidth + 2 * margin
    minimumHeight: mainLayout.Layout.minimumHeight + 2 * margin

    property int nTargetNumber: 10

    GridLayout
    {
        id: mainLayout
        rows: 2
        columns: 2
        flow: GridLayout.LeftToRight
        anchors.fill: parent
        anchors.margins: margin

        GameField{}

        Label
        {
            color: "blue"
            font
            {
                family: "Arial"
                pointSize: 20
                bold: true
            }
            text: nTargetNumber.toString()
        }

        ControlPanel
        {
            Layout.columnSpan: 2
        }
    }

}
