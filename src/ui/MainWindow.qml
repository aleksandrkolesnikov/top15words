import QtQuick 2.15
import QtQuick.Controls 2.15
import QtCharts 2.15
import QtQml 2.15
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
        onAccepted: {
            var path = fileDialog.fileUrl.toString()
            path = path.replace(/^(file:\/{3})/,"")
            editor.acceptText(path)
        }
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

                onAccepted:{
                    editor.border.color = "#87ceeb"
                    dataModel.setFile(editor.text)
                }

                Connections {
                    target: dataModel
                    function onIncorrectFilePassed() {
                        editor.border.color = "red"
                    }
                }

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
            titleFont.bold: true
            Layout.fillWidth: true
            Layout.fillHeight: true
            legend.alignment: Qt.AlignBottom
            legend.font.bold: true
            antialiasing: true

            BarSeries {
                ValueAxis {
                    id: yAxis
                    min: 0
                    max: dataModel.maxFreq
                    labelsFont.bold: true
                }

                BarCategoryAxis {
                    id: barAxis
                    categories: [" "]
                }

                VBarModelMapper {
                    model: dataModel
                    firstBarSetColumn: 0
                    lastBarSetColumn: 14
                    firstRow: 0
                }

                axisY: yAxis
                axisX: barAxis
            }
        }
    }
}
