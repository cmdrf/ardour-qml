add_library(dummy_audiobackend SHARED
	${L}/backends/dummy/dummy_audiobackend.cc
)

target_link_libraries(dummy_audiobackend PUBLIC
	PkgConfig::GLIB2
	PkgConfig::LTC
	pbd
	ardourlib
)

if(APPLE)
	add_library(coreaudio_backend SHARED
		${L}/backends/coreaudio/coreaudio_backend.cc
		${L}/backends/coreaudio/coreaudio_pcmio.cc
		${L}/backends/coreaudio/coremidi_io.cc
	)

	target_link_libraries(coreaudio_backend PUBLIC
		PkgConfig::GLIB2
		PkgConfig::LTC
		pbd
		ardourlib
		"$<LINK_LIBRARY:FRAMEWORK,CoreMIDI>"
	)
endif()
