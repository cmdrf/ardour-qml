import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
	id: strip
	required property var stripable

	color: "darkgrey"

	ColumnLayout {
		anchors.fill: parent
		property real gain: strip.stripable ? strip.stripable.gainControl.value : 0.0
		onGainChanged: if(!gainSlider.pressed) gainSlider.value = gain

		Repeater {
			model: strip.stripable ? strip.stripable.processors : null
			delegate: Text {
				required property var processor
				required property var type

				text: type + ": " + processor.name
			}
		}

		Slider {
			id: gainSlider
			enabled: strip.stripable !== null
			implicitHeight: 300
			implicitWidth: 30
			orientation: Qt.Vertical
			onMoved: strip.stripable.gainControl.value = value
		}
	}
}
