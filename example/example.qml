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
        flickableDirection: Flickable.Flickable.HorizontalAndVerticalFlick
        contentWidth: 2000
        delegate: Row {
            required property var route
            required property int trackNumber

            width: 2000
            height: 60
            Rectangle {
                width: 200
                height: 60
                color: "darkgrey"

                Row {
                    anchors.fill: parent

                    Text {
                        text: trackNumber
                    }

                    Button {
                        contentItem: Text{text: "M"; color: route.muted ? "blue" : (route.mutedByOthersSoloing ? "lightblue" : "black")}
                        onClicked: route.muteControl.value = route.muted ? 0.0 : 1.0
                    }

                    Button {
                        contentItem: Text{text: "S"; color: route.soloed ? "orange" : "black"}
                        onClicked: route.soloControl.value = route.soloed ? 0.0 : 1.0
                    }
                }
            }

            Rectangle {
                color: "lightgrey"
                height: 60
                width: 2000
                Repeater {
                    model: route.playlist
                    delegate: Rectangle {
                        x: region.position.samples / 5000
                        height: 55
                        width: region.length.samples / 5000
                        color: "blue"
                        radius: 3

                        Waveform {
                            anchors.fill: parent
                            audioRegion: region.dataType === Region.AudioType ? region : null
                            visible: region.dataType === Region.AudioType
                        }

                        MidiView {
                            anchors.fill: parent
                            visible: region.dataType === Region.MidiType
                            midiRegion: region.dataType === Region.MidiType ? region : null
                        }
                    }
                }
            }
        }
    }
}
