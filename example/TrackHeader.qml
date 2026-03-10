import QtQuick
import QtQuick.Controls
import Ardour

Rectangle {
	id: header
	required property var route

	signal clicked()

	width: 200
	height: 60
	color: route.selected ? "lightgrey" : "darkgrey"

	MouseArea {
		anchors.fill: parent
		onClicked: (mouse) => {
			header.clicked();
			Ardour.session.selection.selectStripableAndMaybeGroup(header.route, (mouse.modifiers & Qt.ShiftModifier) ? CoreSelection.SelectionToggle : CoreSelection.SelectionSet)
		}
	}

	Row {
		anchors.fill: parent

		Text {
			text: header.route.trackNumber
		}

		Button {
			contentItem: Text{text: "M"; color: header.route.muted ? "blue" : (header.route.mutedByOthersSoloing ? "lightblue" : "black")}
			onClicked: header.route.muteControl.value = header.route.muted ? 0.0 : 1.0
		}

		Button {
			contentItem: Text{text: "S"; color: header.route.soloed ? "orange" : "black"}
			onClicked: header.route.soloControl.value = header.route.soloed ? 0.0 : 1.0
		}
	}
}
