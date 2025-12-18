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

    RowLayout {
        anchors.fill: parent
        Strip {
            stripable: ardour.session.selection.firstSelectedStripable
            Layout.fillHeight: true
            width: 180
        }

    ListView {
        Layout.fillWidth: true
        Layout.fillHeight: true
        model: ardour.session ? ardour.session.tracks : null
        flickableDirection: Flickable.Flickable.HorizontalAndVerticalFlick
        contentWidth: 2000
        delegate: Row {
            required property var route
            required property int trackNumber

            width: 2000
            height: 60

            TrackHeader {
                route: parent.route
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
}
