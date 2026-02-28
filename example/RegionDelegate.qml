import QtQuick
import Ardour

Rectangle {
	required property var region
	required property var route
	required property real samplesPerPixel

	id: regionRect

	x: region.position.samples / samplesPerPixel
	height: sheetView.contentHeight / sheetView.rows // TODO
	width: region.length.samples / samplesPerPixel
	color: selection.regions.contains(region) ? Qt.lighter(route.presentationInfo.color) : route.presentationInfo.color
	radius: 7

	Text {
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: parent.top
		font.pixelSize: 12
		text: parent.region.properties.name
		elide: Text.ElideMiddle
	}

	Waveform {
		anchors.fill: parent
		audioRegion: parent.region.dataType === Region.AudioType ? parent.region : null
		visible: parent.region.dataType === Region.AudioType
		alphaBlending: true
	}

	MidiView {
		anchors.fill: parent
		visible: parent.region.dataType === Region.MidiType
		midiRegion: parent.region.dataType === Region.MidiType ? region : null
	}

	// Ghost rectangle for dragging
	Rectangle {
		id: dragGhost
		visible: false
		width: regionRect.width
		height: regionRect.height
		color: regionRect.color
		opacity: 0.7
		radius: 7
		z: 10
	}

	MouseArea {
		id: mouseArea
		anchors.fill: parent
		drag.target: dragGhost
		drag.axis: Drag.XAxis
		drag.minimumX: -regionRect.x
		drag.maximumX: sheetView.contentWidth - regionRect.width - regionRect.x

		onPressed: (mouse) => {
			if(mouse.modifiers & Qt.ShiftModifier)
				selection.select(parent.region, RegionSelection.Toggle);
			else
				selection.select(parent.region, RegionSelection.ClearAndSelect);

			// Prepare for dragging
			dragGhost.visible = true;
			dragGhost.x = 0;
			dragGhost.y = 0;
		}

		onReleased: (mouse) => {
			if(dragGhost.visible) {
				// Calculate new position in samples
				var newX = dragGhost.x + regionRect.x;
				var newPositionSamples = newX * regionRect.samplesPerPixel;

				// Create new TimePos and update region
				var newPosition = TimePos.fromSamples(newPositionSamples);
				parent.region.setPosition(newPosition);

				// Hide ghost
				dragGhost.visible = false;
			}
		}

		onClicked: (mouse) => {
			if(mouse.modifiers & Qt.ShiftModifier)
				selection.select(parent.region, RegionSelection.Toggle);
			else
				selection.select(parent.region, RegionSelection.ClearAndSelect);
		}
	}
}
