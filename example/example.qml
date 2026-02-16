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
    color: "#555555"

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

        MainView {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
