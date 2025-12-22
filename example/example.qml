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

    menuBar: MenuBar {
        Menu {
           title: qsTr("&File")
           Action {
               text: qsTr("&New...")
               onTriggered: createDialog.open()
           }
           Action {
               text: qsTr("&Open...")
               onTriggered: openDialog.open()
           }
           Action { text: qsTr("&Save") }
           Action { text: qsTr("Save &As...") }
           MenuSeparator { }
           Action { text: qsTr("&Quit") }
       }
       Menu {
           title: qsTr("&Edit")
           Action { text: qsTr("Cu&t") }
           Action { text: qsTr("&Copy") }
           Action { text: qsTr("&Paste") }
       }
       Menu {
           title: "Track"
           Action {
               text: "New Audio Track..."
               onTriggered: newAudioTrackDialog.open()
           }
       }
    }

    header: ToolBar {
        RowLayout {
            anchors.fill: parent

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

    FileDialog {
        id: createDialog
        fileMode: FileDialog.SaveFile
        onAccepted: {
            const url = new URL(selectedFile);
            const path = url.pathname;
            const lastItem = path.substring(path.lastIndexOf('/') + 1);
            ardour.createSession(path, lastItem, 48000)
        }
    }

    NewAudioTrackDialog {
        id: newAudioTrackDialog
        onAccepted: ardour.session.newAudioTrack(inputChannels, outputChannels)
    }

    RowLayout {
        anchors.fill: parent
        Strip {
            stripable: ardour.session.selection.firstSelectedStripable
            Layout.fillHeight: true
            width: 180
        }

        VerticalHeaderView {
            Layout.fillHeight: true
            width: 200

            syncView: sheetView
            model: sheetView.model
            delegate: TrackHeader {
                implicitWidth: 200
                required property int trackNumber

            }
        }

        TableView {
            id: sheetView
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: ardour.session ? ardour.session.tracks : null
            clip: true
            contentWidth: ardour.session.currentEnd.samples / 5000
            delegate: Rectangle {
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
