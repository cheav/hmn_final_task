import QtQuick 2.7

Rectangle
{
    property alias imageSource: image.source

    width: 100
    height: 50

    Image
    {
        id: image
        smooth: true
        anchors.centerIn: parent
    }

    ParallelAnimation
    {
        NumberAnimation
        {
            target: image
            properties: "scale"
            from: 0.1
            to: 1.0
            duration: 2000
            easing.type: Easing.InOutCubic
        }

        NumberAnimation
        {
            target: image
            properties: "opacity"
            from: 1
            to: 0.3
            duration: 2000
        }

        running: true
    }

}
