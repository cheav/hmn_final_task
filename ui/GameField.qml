import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import com.harman.game 1.0

GroupBox
{
    id: gridBox
    Layout.fillWidth: true
    Layout.fillHeight: true
    //
    GridView
    {
        id: view
        flow: GridView.FlowLeftToRight

        QmlModel{ id: qmlModel }

        GameModel
        {
            id: gameModel
            rows: 6
            columns: 10
            property int size: rows * columns
        }
        property int nButtonSize: 60
        property int nButtonWidth: nButtonSize
        property int nButtonHeight: nButtonSize

        width: gameModel.columns * nButtonWidth
        height: gameModel.rows * nButtonHeight
        cellWidth: width / gameModel.columns
        cellHeight: height / gameModel.rows

        model: gameModel //qmlModel
        delegate: ButtonNumber
        {
            buttonVisible: true
            text: number.toString()
            //gameModel.data(view.currentIndex).toString()
        }
    }

} // GroupBox
