import QtQuick
import QtQuick.Controls
import Ardour

Rectangle {
	required property var route

	width: 200
	height: 60
	color: route.selected ? "lightgrey" : "darkgrey"

	MouseArea {
		anchors.fill: parent
		onClicked: (mouse) => {
			ardour.session.selection.selectStripableAndMaybeGroup(route, (mouse.modifiers & Qt.ShiftModifier) ? CoreSelection.SelectionToggle : CoreSelection.SelectionSet)
		}
	}

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
