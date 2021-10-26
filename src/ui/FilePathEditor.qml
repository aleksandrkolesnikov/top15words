import QtQuick 2.15

Rectangle {
    id: filePathEditor
    property alias text: textInput.text
    signal accepted
    border.color: "#87ceeb"

    function acceptText(newText) {
        text = newText
        accepted()
    }

    TextInput {
        id: textInput
        text: "Type..."
        anchors.fill: parent
        verticalAlignment: TextInput.AlignVCenter
    }

    Component.onCompleted: {
        textInput.accepted.connect(filePathEditor.accepted)
    }
}
