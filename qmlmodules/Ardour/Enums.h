#ifndef ENUMS_H
#define ENUMS_H

#include <ardour/types.h>

#include <QtQml>

namespace Ardour
{
Q_NAMESPACE
QML_ELEMENT

enum Placement
{
	PreFaderPlacement = ARDOUR::PreFader,
	PostFaderPlacement = ARDOUR::PostFader
};
Q_ENUM_NS(Placement);

enum AlignStyle
{
	CaptureTimeAlignStyle = ARDOUR::CaptureTime,
	ExistingMaterialAlignStyle = ARDOUR::ExistingMaterial
};
Q_ENUM_NS(AlignStyle);

enum TrackMode {
	NormalTrackMode = ARDOUR::Normal,
	NonLayeredTrackMode = ARDOUR::NonLayered,
	DestructiveTrackMode = ARDOUR::Destructive,
};
Q_ENUM_NS(TrackMode);

enum RecordState
{
	DisabledRecordState = ARDOUR::Disabled,
	EnabledRecordState = ARDOUR::Enabled,
	RecordingRecordState = ARDOUR::Recording
};
Q_ENUM_NS(RecordState);

}

#endif // ENUMS_H
