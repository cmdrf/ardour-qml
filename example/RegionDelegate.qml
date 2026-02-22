import QtQuick
import Ardour

Rectangle {
	required property var region
	required property var route
	required property real samplesPerPixel

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

	MouseArea {
		anchors.fill: parent
		onClicked: (mouse) => {
			if(mouse.modifiers & Qt.ShiftModifier)
				selection.select(parent.region, RegionSelection.Toggle);
			else
				selection.select(parent.region, RegionSelection.ClearAndSelect);
		}
	}
}
