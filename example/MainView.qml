import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Ardour

GridLayout {
	id: mainView

	property real samplesPerPixel: ardour.session.currentEnd.samples / sheetView.contentWidth

	columns: 2

	Item {
		width: 200
		height: 50
	}

	HorizontalHeaderView {
		Layout.fillWidth: true
		syncView: sheetView
		height: 50
		clip: true
		delegate: Rectangle {
			color: "white"
			height: 50
			implicitWidth: sheetView.contentWidth
			Repeater {
				model: TempoMap
				delegate: Rectangle {
					width: 1
					height: 50
					color: "green"
					x: time.samples / 5000

					Text {
						visible: type == TempoMap.TempoPointType
						anchors.left: parent.left
						anchors.top: parent.top
						text: noteTypesPerMinute
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
	}

	VerticalHeaderView {
		Layout.fillHeight: true
		width: 200
		clip: true

		syncView: sheetView
		model: sheetView.model
		delegate: TrackHeader {
			implicitWidth: 200
			required property int trackNumber

		}
	}

	PinchArea {
		Layout.fillWidth: true
		Layout.fillHeight: true


		property real initialWidth
		property real initialHeight

		onPinchStarted: {
			initialWidth = sheetView.contentWidth;
			initialHeight = sheetView.contentHeight;
		}
		onPinchUpdated: (pinch) => {
			// adjust content pos due to drag TODO:
			sheetView.contentX += pinch.previousCenter.x - pinch.center.x;
			sheetView.contentY += pinch.previousCenter.y - pinch.center.y;

			// resize content:
			sheetView.resizeContent(initialWidth * pinch.scale, initialHeight /*TODO*/, Qt.point(pinch.center.x + sheetView.contentX, pinch.center.y + sheetView.contentY));
		}
		onPinchFinished: {
			// Move its content within bounds:
			sheetView.returnToBounds()
		}

		TableView {
			id: sheetView
			anchors.fill: parent

			model: ardour.session ? ardour.session.tracks : null
			clip: true
			contentWidth: 5000
			contentHeight: 1000
			delegate: Rectangle {
				color: "lightgrey"
				height: sheetView.contentHeight / sheetView.rows // TODO
				implicitWidth: sheetView.contentWidth
				Repeater {
					model: route.playlist
					delegate: Rectangle {
						x: region.position.samples / mainView.samplesPerPixel
						height: sheetView.contentHeight / sheetView.rows // TODO
						width: region.length.samples / mainView.samplesPerPixel
						color: "blue"
						radius: 3

						Waveform {
							anchors.fill: parent
							audioRegion: region.dataType === Region.AudioType ? region : null
							visible: region.dataType === Region.AudioType
						}

						MidiView {
							anchors.fill: parent
							visible: region.dataType === Region.MidiType
							midiRegion: region.dataType === Region.MidiType ? region : null
						}
					}
				}
			}
		}
	}
}
