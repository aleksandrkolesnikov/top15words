import QtQuick 2.15
import QtQuick.Controls 2.15
import QtCharts 2.15
import QtQml 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

ApplicationWindow
{
    visible: true
    title: "Top 15 Words"
    height: 700
    width: 850

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: shortcuts.home
        selectMultiple: false
        onAccepted: {
            dataModel.setFile(fileDialog.fileUrl)
            //console.log("You chose: " + fileDialog.fileUrls)
            //Qt.quit()
        }
        onRejected: {
            //console.log("Canceled")
            //Qt.quit()
        }
        //Component.onCompleted: visible = true
    }

    ColumnLayout
    {
        anchors.fill: parent

        Button
        {
            id: text
            text: "Select File"
            Layout.alignment: Qt.AlignCenter
            Layout.preferredWidth: 120
            Layout.preferredHeight: 50

            onClicked: fileDialog.visible = true
        }

        ChartView
        {
            id: chartView
            title: "Top 15 Words"
            //anchors.fill: parent
            legend.alignment: Qt.AlignBottom
            antialiasing: true
            Layout.alignment: Qt.AlignBottom
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: parent.height - 10

            BarSeries
            {
                ValueAxis
                {
                    id: yAxis
                    min: 0
                    max: dataModel.maxFreq
                }

                BarCategoryAxis
                {
                    id: barAxis
                    categories: [" "]
                }

                VBarModelMapper
                {
                    model: dataModel
                    firstBarSetColumn: 0
                    lastBarSetColumn: 15
                    firstRow: 0
                }

                axisY: yAxis
                axisX: barAxis
            }
        }
    }
}
