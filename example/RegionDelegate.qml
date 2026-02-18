import QtQuick
import Ardour

Rectangle {
	required property var region

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
