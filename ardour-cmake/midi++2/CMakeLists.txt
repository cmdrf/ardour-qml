add_library(midi++2 STATIC
	${L}/midi++2/channel.cc
	${L}/midi++2/ipmidi_port.cc
	${L}/midi++2/midi.cc
	${L}/midi++2/midnam_patch.cc
	${L}/midi++2/mmc.cc
	${L}/midi++2/mtc.cc
	${L}/midi++2/parser.cc
	${L}/midi++2/port.cc
)

target_include_directories(midi++2 PUBLIC
	${L}/midi++2
)

target_link_libraries(midi++2 PUBLIC
	pbd
	temporal
)

set_property(TARGET midi++2 PROPERTY POSITION_INDEPENDENT_CODE ON)
