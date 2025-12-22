import QtQuick.Controls
import QtQuick.Layouts

Dialog {
	readonly property int inputChannels: inputChannelCombo.currentIndex + 1
	readonly property int outputChannels: outputChannelCombo.currentIndex + 1

	standardButtons: Dialog.Ok | Dialog.Cancel

	GridLayout {
		columns: 2

		Label {
			text: "Input Channels"
		}

		ComboBox {
			id: inputChannelCombo
			model: ["1", "2"]
		}

		Label {
			text: "Output Channels"
		}

		ComboBox {
			id: outputChannelCombo
			model: ["1", "2"]
		}
	}
}
