import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
	required property var stripable

	color: "darkgrey"

	ColumnLayout {
		anchors.fill: parent
		property real gain: stripable ? stripable.gainControl.value : 0.0
		onGainChanged: if(!gainSlider.pressed) gainSlider.value = gain

		Repeater {
			model: stripable.processors
			delegate: Text {
				text: type + ": " + processor.name
			}
		}

		Slider {
			id: gainSlider
			enabled: stripable !== null
			height: 300
			width: 30
			orientation: Qt.Vertical
			onMoved: stripable.gainControl.value = value
		}
	}
}
