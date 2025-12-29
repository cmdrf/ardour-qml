#include "AudioTrigger.h"
#include "QtBridgeUi.h"

AudioTrigger::AudioTrigger(QObject* parent, std::shared_ptr<ARDOUR::AudioTrigger> audioTrigger)
	: Trigger{parent, audioTrigger}
{
	QtBridgeUi& b = QtBridgeUi::instance();

	b.connect(audioTrigger->PropertyChanged, this, &AudioTrigger::updateProperties);
}

void AudioTrigger::updateProperties(const PBD::PropertyChange& change)
{
	if(change.contains(ARDOUR::Properties::stretch_mode))
		Q_EMIT stretchModeChanged();
}

