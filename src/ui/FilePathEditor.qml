import QtQuick 2.15

Rectangle {
    property alias text: textInput.text
    border.color: "#87ceeb"

    TextInput {
        id: textInput
        text: "Type..."
        anchors.fill: parent

        //onTextChanged: color = "red"
    }
}
