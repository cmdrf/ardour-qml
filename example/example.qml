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

    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            ToolButton {
                text: "New session"
                onClicked: ardour.createSession("/tmp/lala", "hallo", 48000)
            }

            ToolButton {
                text: "Open session"
                onClicked: openDialog.open()
            }

            ToolButton {
                icon.name: "media-playback-start"
                icon.color: ardour.session && ardour.session.transportSpeed === 0.0 ? "black" : "green"
                enabled: ardour.session != null
                onClicked: ardour.session.requestRoll()
            }

            ToolButton {
                icon.name: "media-playback-stop"
                enabled: ardour.session != null
                onClicked: ardour.session.requestStop()
            }

            Label {
                text: ardour.session.transportSample
            }

            Item {
                Layout.fillWidth: true
            }
        }
    }

    Ardour {
        id: ardour
    }

    FileDialog {
        id: openDialog
        nameFilters: ["Ardour files (*.ardour)"]
        onAccepted: ardour.loadSession(openDialog.selectedFile)
    }

    ListView {
        anchors.fill: parent
        model: ardour.session ? ardour.session.tracks : null
        delegate: Rectangle {
            required property var route
            required property int trackNumber
            color: "lightgreen"
            width: 1280
            height: 40
            Text {
                text: trackNumber
            }

            Repeater {
                model: route.playlist
                delegate: Rectangle {
                    x: region.position.ticks / 1000
                    height: 30
                    width: region.length.ticks / 1000
                    color: "blue"
                    radius: 3

                    Waveform {
                        anchors.fill: parent
                        audioRegion: region
                    }
                }
            }
        }
    }
}
