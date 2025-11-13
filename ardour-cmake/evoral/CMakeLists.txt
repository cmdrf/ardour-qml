set(E ${L}/evoral)

add_library(evoral
	${E}/Control.cc
	${E}/ControlList.cc
	${E}/ControlSet.cc
	${E}/Curve.cc
	${E}/debug.cc
	${E}/Event.cc
	${E}/Note.cc
	${E}/Sequence.cc
	${E}/SMF.cc
	${E}/libsmf/smf.c
	${E}/libsmf/smf_decode.c
	${E}/libsmf/smf_load.c
	${E}/libsmf/smf_save.c
	${E}/libsmf/smf_tempo.c
)

target_include_directories(evoral PUBLIC
	${L}/evoral
	${Boost_INCLUDE_DIRS}
)

target_link_libraries(evoral PUBLIC
	PkgConfig::GLIB2
	pbd
	temporal
)

target_compile_definitions(evoral PRIVATE
	SMF_VERSION="1.2"
)
