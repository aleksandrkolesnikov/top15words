import QtQuick 2.15
import QtQuick.Controls 2.15
import QtCharts 2.15
//import QtQml 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

ApplicationWindow {
    visible: true
    title: "Top 15 Words"
    height: 700
    width: 850

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: shortcuts.home
        selectMultiple: false
        onAccepted: editor.text = fileDialog.fileUrl
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.topMargin: 5
        anchors.bottomMargin: 5
        anchors.leftMargin: 5
        anchors.rightMargin: 5

        RowLayout {

            FilePathEditor {
                id: editor
                Layout.fillWidth: true
                Layout.preferredHeight: 40

                onTextChanged: dataModel.setFile(editor.text)
            }

            Button {
                text: "Browse..."
                Layout.alignment: Qt.AlignRight
                Layout.preferredWidth: 120
                Layout.preferredHeight: 40

                onClicked: fileDialog.visible = true
            }
        }

        ChartView {
            id: chartView
            title: "Top 15 Words"
            Layout.fillWidth: true
            Layout.fillHeight: true
            legend.alignment: Qt.AlignBottom
            antialiasing: true

            BarSeries {
                ValueAxis {
                    id: yAxis
                    min: 0
                    max: dataModel.maxFreq
                }

                BarCategoryAxis {
                    id: barAxis
                    categories: [" "]
                }

                VBarModelMapper {
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
