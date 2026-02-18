import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Shapes
import Ardour

GridLayout {
	id: mainView

	property real samplesPerPixel: ardour.session.currentEnd.samples / sheetView.contentWidth

	columns: 2

	Item {
		width: 200
		height: 30
		implicitHeight: 30
	}

	HorizontalHeaderView {
		Layout.fillWidth: true
		syncView: sheetView
		height: 30
		implicitHeight: 30
		clip: true
		interactive: false
		delegate: Item {
			height: 30
			implicitWidth: sheetView.contentWidth
			Repeater {
				model: TempoMap
				delegate: Rectangle {
					width: 1
					height: 30
					color: "green"
					x: time.samples / mainView.samplesPerPixel

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

		// Play position triangle
		Shape {
			width: 20
			height: 10
			x: ardour.session.transportSample / mainView.samplesPerPixel - 10
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
			anchors.fill: parent
			onPressed: (mouse) => {
				if(mouse.button === Qt.LeftButton)
					ardour.session.requestLocate(mouse.x * mainView.samplesPerPixel);
			}
			onPositionChanged: (mouse) => {
				if(mouse.buttons & Qt.LeftButton)
					ardour.session.requestLocate(mouse.x * mainView.samplesPerPixel);
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
			onClicked: selection.select(route.playlist, RegionSelection.ClearAndSelect)
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
			delegate: Item { // One track or playlist
				height: sheetView.contentHeight / sheetView.rows // TODO
				implicitWidth: sheetView.contentWidth

				MouseArea {
					anchors.fill: parent
					onClicked: selection.clear()
				}

				Repeater {
					model: route.playlist
					delegate: Rectangle { // One region
						x: region.position.samples / mainView.samplesPerPixel
						height: sheetView.contentHeight / sheetView.rows // TODO
						width: region.length.samples / mainView.samplesPerPixel
						color: selection.regions.contains(region) ? Qt.lighter(route.presentationInfo.color) : route.presentationInfo.color
						radius: 7

						Text {
							anchors.left: parent.left
							anchors.right: parent.right
							anchors.top: parent.top
							font.pixelSize: 12
							text: region.properties.name
							elide: Text.ElideMiddle
						}

						Waveform {
							anchors.fill: parent
							audioRegion: region.dataType === Region.AudioType ? region : null
							visible: region.dataType === Region.AudioType
							alphaBlending: true
						}

						MidiView {
							anchors.fill: parent
							visible: region.dataType === Region.MidiType
							midiRegion: region.dataType === Region.MidiType ? region : null
						}

						MouseArea {
							anchors.fill: parent
							onClicked: (mouse) => {
								if(mouse.modifiers & Qt.ShiftModifier)
									selection.select(region, RegionSelection.Toggle);
								else
								   selection.select(region, RegionSelection.ClearAndSelect);
							}
						}
					}
				}
			}

			Rectangle {
				width: 1
				x: ardour.session.transportSample / mainView.samplesPerPixel - width * 0.5
				y: 0
				z: 10
				height: 10000
				color: "black"
			}
		}

		RegionSelection {
			id: selection
		}
	}
}
