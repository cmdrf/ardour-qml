import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Ardour

GridLayout {
	property real songWidth: ardour.session.currentEnd.samples / 5000

	columns: 2

	Item {
		width: 200
		height: 50
	}

	HorizontalHeaderView {
		Layout.fillWidth: true
		syncView: sheetView
		contentWidth: songWidth
		height: 50
		clip: true
		delegate: Rectangle {
			color: "white"
			height: 50
			implicitWidth: songWidth
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
						text: noteTypesPerValue
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

	TableView {
		id: sheetView
		Layout.fillWidth: true
		Layout.fillHeight: true
		model: ardour.session ? ardour.session.tracks : null
		clip: true
		contentWidth: songWidth
		delegate: Rectangle {
			color: "lightgrey"
			height: 60
			implicitWidth: songWidth
			Repeater {
				model: route.playlist
				delegate: Rectangle {
					x: region.position.samples / 5000
					height: 55
					width: region.length.samples / 5000
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
