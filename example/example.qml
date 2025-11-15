import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs
import Ardour

ApplicationWindow {
    width: 1280
    height: 720
    visible: true
    title: qsTr("Example Project")

    Ardour {
        id: ardour
    }

    FileDialog {
        id: openDialog
        nameFilters: ["Ardour files (*.ardour)"]
        onAccepted: ardour.loadSession(openDialog.selectedFile)
    }

    GridLayout {
        Button {
            text: "New session"
            onClicked: ardour.createSession("/tmp/lala", "hallo", 48000)
        }

        Button {
            text: "Open session"
            onClicked: openDialog.open()
        }

        Button {
            icon.name: "media-playback-start"
            icon.color: ardour.session && ardour.session.transportSpeed === 0.0 ? "black" : "green"
            enabled: ardour.session != null
            onClicked: ardour.session.requestRoll()
        }

        Button {
            icon.name: "media-playback-stop"
            enabled: ardour.session != null
            onClicked: ardour.session.requestStop()
        }

        ListView {
            width: 100
            height: 200
            model: ardour.session ? ardour.session.routes : null
            delegate: Rectangle {
                required property var route
                required property int trackNumber
                color: "lightgreen"
                width: 40
                height: 40
                Text {
                    text: trackNumber
                }
            }
        }
    }
}
