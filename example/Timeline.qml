import QtQuick
import QtQuick.Shapes
import QtQuick.Controls
import Ardour

HorizontalHeaderView {
	id: timeline
	required property var sheetView
	readonly property real samplesPerPixel: Ardour.session.currentEnd.samples / contentWidth
	property bool loopEnabled: false

	signal loopSet

	function roundBeats(samples) {
		let beats = Ardour.session.tempoMap.quartersAt(TimePos.fromSamples(samples));
		let roundedBeats = beats.roundToBeat();
		return Ardour.session.tempoMap.sampleAt(roundedBeats);
	}

	syncView: sheetView
	implicitHeight: 30
	contentHeight: 30
	clip: true
	interactive: false
	textRole: ""
	delegate: Item {
		height: 30
		implicitWidth: timeline.contentWidth
		Repeater {
			model: Ardour.session.tempoMap
			delegate: Rectangle {
				required property timepos time
				required property int type
				required property real noteTypesPerMinute
				required property int divisionsPerBar
				required property int noteValue

				width: 1
				height: 30
				color: "green"
				x: time.samples / timeline.samplesPerPixel
				z: 1

				Text {
					visible: parent.type == TempoMap.TempoPointType
					anchors.left: parent.left
					anchors.top: parent.top
					text: {
						// Print at most one decimal place:
						let bpmStr = parent.noteTypesPerMinute.toFixed(1);
						if(bpmStr.endsWith(".0"))
							bpmStr = bpmStr.slice(0, -2);
						bpmStr;
					}
				}

				Text {
					visible: parent.type == TempoMap.MeterPointType
					anchors.left: parent.left
					anchors.bottom: parent.bottom
					text: parent.divisionsPerBar + "/" + parent.noteValue
				}
			}
		}
	}

	Repeater {
		model: GridModel {
			startSamples: timeline.visibleArea.xPosition * Ardour.session.currentEnd.samples
			endSamples: (timeline.visibleArea.xPosition + timeline.visibleArea.widthRatio) * Ardour.session.currentEnd.samples
		}
		delegate: Rectangle {
			required property timepos time
			required property int bbtBars
			required property int bbtBeats

			width: 1
			height: timeline.height
			color: "#777777"
			x: time.samples / timeline.samplesPerPixel

			Text {
				anchors.left: parent.right
				anchors.top: parent.top
				text: parent.bbtBars
				visible: parent.bbtBeats == 1
			}
		}
	}

	// Play position triangle
	Shape {
		width: 20
		height: 10
		x: Ardour.session.transportSample / timeline.samplesPerPixel - 10
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

	// Actual Loop
	Rectangle {
		opacity: 0.5
		color: timeline.loopEnabled && !ghostLoop.visible ? "yellow" : "black"
		anchors.top: parent.top
		anchors.bottom: parent.verticalCenter
		visible: Ardour.session.autoLoopLocation !== null
		x: Ardour.session.autoLoopLocation.start.samples / timeline.samplesPerPixel
		width: (Ardour.session.autoLoopLocation.end.samples - Ardour.session.autoLoopLocation.start.samples) / timeline.samplesPerPixel
	}

	// Ghost loop
	Rectangle {
		id: ghostLoop
		property real startSamples: 0
		property real endSamples: 48000

		visible: false
		color: "yellow"
		opacity: 0.5
		anchors.top: parent.top
		anchors.bottom: parent.verticalCenter
		x: Math.min(startSamples, endSamples) / timeline.samplesPerPixel
		width: Math.abs(endSamples - startSamples) / timeline.samplesPerPixel
	}

	// Mouse area to set loop
	MouseArea {
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.verticalCenter
		anchors.top: parent.top

		onPressed: (mouse) => {
			ghostLoop.visible = true;
			ghostLoop.startSamples = ghostLoop.endSamples = timeline.roundBeats(mouse.x * timeline.samplesPerPixel);
		}
		onPositionChanged: (mouse) => {
			ghostLoop.endSamples = timeline.roundBeats(mouse.x * timeline.samplesPerPixel);
		}
		onReleased: (mouse) => {
			ghostLoop.visible = false;
			let start = Math.min(ghostLoop.startSamples, ghostLoop.endSamples);
			let end = Math.max(ghostLoop.startSamples, ghostLoop.endSamples);

			Ardour.session.setAutoLoopLocation(TimePos.fromSamples(start), TimePos.fromSamples(end));
			timeline.loopSet();
		}
	}

	// Mouse area to change the play position
	MouseArea {
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		anchors.top: parent.verticalCenter

		onPressed: (mouse) => {
			if(mouse.button === Qt.LeftButton)
				Ardour.session.requestLocate(timeline.roundBeats(mouse.x * timeline.samplesPerPixel));
		}
		onPositionChanged: (mouse) => {
			if(mouse.buttons & Qt.LeftButton)
				Ardour.session.requestLocate(timeline.roundBeats(mouse.x * timeline.samplesPerPixel));
		}
	}
}

