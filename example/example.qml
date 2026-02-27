import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs
import Ardour

ApplicationWindow {
    id: mainWindow
    width: 1280
    height: 720
    visible: true
    title: qsTr("Example Project")
    color: "#555555"

    menuBar: MenuBar {
        Menu {
           title: qsTr("&File")
           Action {
               text: qsTr("&New...")
               onTriggered: createDialog.open()
               enabled: !Ardour.session
           }
           Action {
               text: qsTr("&Open...")
               onTriggered: openDialog.open()
               enabled: !Ardour.session
           }
           Action {
               text: qsTr("&Save")
               enabled: Ardour.session && Ardour.session.dirty
               onTriggered: Ardour.session.saveState()
           }
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

            // See https://specifications.freedesktop.org/icon-naming/latest/#names for icons
            ToolButton {
                icon.name: "media-playback-start"
                icon.color: Ardour.session && Ardour.session.transportSpeed === 0.0 ? "black" : "green"
                enabled: Ardour.session !== null
                onClicked: mainWindow.loopEnabled ? Ardour.session.requestPlayLoop(true) : Ardour.session.requestRoll()
            }

            ToolButton {
                icon.name: "media-playback-stop"
                enabled: Ardour.session !== null
                onClicked: Ardour.session.requestStop()
            }

            ToolButton {
                icon.name: "media-playlist-repeat"
                icon.color: Ardour.session.extra.loopEnabled ? "yellow" : "black"
                onClicked: Ardour.session.extra.loopEnabled = !Ardour.session.extra.loopEnabled
            }

            Label {
                text: Ardour.session.transportSample
            }

            Item {
                Layout.fillWidth: true
            }
        }
    }

    FileDialog {
        id: openDialog
        nameFilters: ["Ardour files (*.ardour)"]
        onAccepted: Ardour.loadSession(openDialog.selectedFile)
    }

    FileDialog {
        id: createDialog
        fileMode: FileDialog.SaveFile
        onAccepted: {
            const url = new URL(selectedFile);
            const path = url.pathname;
            const lastItem = path.substring(path.lastIndexOf('/') + 1);
            Ardour.createSession(path, lastItem, 48000)
        }
    }

    NewAudioTrackDialog {
        id: newAudioTrackDialog
        onAccepted: Ardour.session.newAudioTrack(inputChannels, outputChannels)
    }

    RowLayout {
        anchors.fill: parent
        Strip {
            stripable: Ardour.session.selection.firstSelectedStripable
            Layout.fillHeight: true
            implicitWidth: 180
        }

        MainView {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
