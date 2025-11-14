import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import Ardour

ApplicationWindow {
    width: 640
    height: 400
    visible: true
    title: qsTr("Example Project")

    Ardour {
        id: ardour
    }

    GridLayout {

        Button {
            text: "Create session"
            onClicked: ardour.createSession("/tmp", "hallo", 48000)
        }

        Button {
            text: "Play"
            onClicked: ardour.session.requestRoll()
        }
    }
}
