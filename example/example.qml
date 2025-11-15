import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import Ardour

ApplicationWindow {
    width: 1280
    height: 720
    visible: true
    title: qsTr("Example Project")

    Ardour {
        id: ardour
    }

    GridLayout {

        Button {
            text: "Create session"
            onClicked: ardour.createSession("/tmp/lala", "hallo", 48000)
        }

        Button {
            text: "Play"
            onClicked: ardour.session.requestRoll()
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
