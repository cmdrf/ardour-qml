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
		height: 30
		implicitHeight: 30
	}

	Timeline {
		sheetView: sheetView
		Layout.fillWidth: true
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
					delegate: RegionDelegate {

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
