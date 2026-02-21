import QtQuick
import QtQuick.Shapes
import QtQuick.Controls
import Ardour

HorizontalHeaderView {
	required property var sheetView
	readonly property real samplesPerPixel: Ardour.session.currentEnd.samples / contentWidth

	syncView: sheetView
	height: 30
	implicitHeight: 30
	clip: true
	interactive: false
	textRole: ""
	delegate: Item {
		height: 30
		implicitWidth: contentWidth
		Repeater {
			model: Ardour.session.tempoMap
			delegate: Rectangle {
				width: 1
				height: 30
				color: "green"
				x: time.samples / samplesPerPixel
				z: 1

				Text {
					visible: type == TempoMap.TempoPointType
					anchors.left: parent.left
					anchors.top: parent.top
					text: {
						// Print at most one decimal place:
						let bpmStr = noteTypesPerMinute.toFixed(1);
						if(bpmStr.endsWith(".0"))
							bpmStr = bpmStr.slice(0, -2);
						bpmStr;
					}
				}

				Text {
					visible: type == TempoMap.MeterPointType
					anchors.left: parent.left
					anchors.bottom: parent.bottom
					text: divisionsPerBar + "/" + noteValue
				}
			}
		}
	}

	Repeater {
		model: GridModel {
			startSamples: 0
			endSamples: Ardour.session.currentEnd.samples
		}
		delegate: Rectangle {
			required property timepos time
			required property int bbtBars
			required property int bbtBeats

			width: 1
			height: 30
			color: "#777777"
			x: time.samples / samplesPerPixel

			Text {
				anchors.left: parent.right
				anchors.top: parent.top
				text: bbtBars
				visible: bbtBeats == 1
			}
		}
	}

	// Play position triangle
	Shape {
		width: 20
		height: 10
		x: Ardour.session.transportSample / samplesPerPixel - 10
		y: 20
		z: 10

		ShapePath {
			fillColor: "black"
			startX: 0
			startY: 0
			PathLine { x: 20; y: 0 }
			PathLine { x: 10; y: 10 }
			PathLine { x: 0; y: 0 }
		}
	}

	// Mouse area to change the play position
	MouseArea {
		function roundBeats(samples) {
			let beats = Ardour.session.tempoMap.quartersAt(TimePos.fromSamples(samples));
			let roundedBeats = beats.roundToBeat();
			return Ardour.session.tempoMap.sampleAt(roundedBeats);
		}

		anchors.fill: parent
		onPressed: (mouse) => {
			if(mouse.button === Qt.LeftButton)
				Ardour.session.requestLocate(roundBeats(mouse.x * samplesPerPixel));
		}
		onPositionChanged: (mouse) => {
			if(mouse.buttons & Qt.LeftButton)
				Ardour.session.requestLocate(roundBeats(mouse.x * samplesPerPixel));
		}
	}
}

