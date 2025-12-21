#ifndef CHANCOUNT_H
#define CHANCOUNT_H

#include <ardour/chan_count.h>

#include <QtQml>

class ChanCount
{
	Q_GADGET

	Q_PROPERTY(int audioChannels READ audioChannels WRITE setAudioChannels FINAL)
	Q_PROPERTY(int midiChannels READ midiChannels WRITE setMidiChannels FINAL)

	QML_VALUE_TYPE(chanCount)

public:
	ARDOUR::ChanCount& chanCount() {return m_chanCount;}
	const ARDOUR::ChanCount& chanCount() const {return m_chanCount;}

	int audioChannels() const {return m_chanCount.n_audio();}
	void setAudioChannels(int newAudioChannels) {m_chanCount.set_audio(newAudioChannels);}
	int midiChannels() const {return m_chanCount.n_midi();}
	void setMidiChannels(int newMidiChannels) {m_chanCount.set_midi(newMidiChannels);}

private:
	ARDOUR::ChanCount m_chanCount;
};

#endif // CHANCOUNT_H
