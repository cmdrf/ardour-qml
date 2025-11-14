add_library(pan1in2out SHARED
	${L}/panners/1in2out/panner_1in2out.cc
)

target_link_libraries(pan1in2out PRIVATE
	PkgConfig::GLIB2
	pbd
	evoral
	ardourlib
)

add_library(pan2in2out SHARED
	${L}/panners/2in2out/panner_2in2out.cc
)

target_link_libraries(pan2in2out PRIVATE
	PkgConfig::GLIB2
	pbd
	evoral
	ardourlib
)

add_library(panbalance SHARED
	${L}/panners/stereobalance/panner_balance.cc
)

target_link_libraries(panbalance PRIVATE
	PkgConfig::GLIB2
	pbd
	evoral
	ardourlib
)

add_library(panvbap SHARED
	${L}/panners/vbap/vbap_speakers.cc
	${L}/panners/vbap/vbap.cc
)

target_link_libraries(panvbap PRIVATE
	pbd
	ardourlib
)
