add_library(appleutility SHARED
	${L}/appleutility/CoreAudio/PublicUtility/AUOutputBL.cpp
	${L}/appleutility/CoreAudio/PublicUtility/AUParamInfo.cpp
	${L}/appleutility/CoreAudio/PublicUtility/CAAudioChannelLayout.cpp
	${L}/appleutility/CoreAudio/PublicUtility/CAAudioChannelLayoutObject.cpp
	${L}/appleutility/CoreAudio/PublicUtility/CAAudioUnit.cpp
	${L}/appleutility/CoreAudio/PublicUtility/CAAUParameter.cpp
	${L}/appleutility/CoreAudio/PublicUtility/CABufferList.cpp
	${L}/appleutility/CoreAudio/PublicUtility/CACFDictionary.cpp
	${L}/appleutility/CoreAudio/PublicUtility/CACFNumber.cpp
	${L}/appleutility/CoreAudio/PublicUtility/CACFString.cpp
	${L}/appleutility/CoreAudio/PublicUtility/CAComponent.cpp
	${L}/appleutility/CoreAudio/PublicUtility/CAComponentDescription.cpp
	${L}/appleutility/CoreAudio/PublicUtility/CADebugMacros.cpp
	${L}/appleutility/CoreAudio/PublicUtility/CAStreamBasicDescription.cpp
	${L}/appleutility/CoreAudio/PublicUtility/CAXException.cpp
)

target_include_directories(appleutility PUBLIC
	${L}/appleutility/CoreAudio/PublicUtility
)

target_link_libraries(appleutility PUBLIC
	"$<LINK_LIBRARY:FRAMEWORK,AudioToolbox>"
	"$<LINK_LIBRARY:FRAMEWORK,CoreServices>"
	"$<LINK_LIBRARY:FRAMEWORK,CoreAudio>"
)
