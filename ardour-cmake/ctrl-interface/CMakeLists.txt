add_library(ctrl-interface
	${L}/ctrl-interface/control_protocol/basic_ui.cc
	${L}/ctrl-interface/control_protocol/control_protocol.cc
	${L}/ctrl-interface/midi_surface/midi_byte_array.cc
	${L}/ctrl-interface/midi_surface/midi_surface.cc
)

target_link_libraries(ctrl-interface PUBLIC
	pbd
	temporal
	ardourlib
)

target_include_directories(ctrl-interface PUBLIC
	${L}/ctrl-interface/control_protocol
	${L}/ctrl-interface/midi_surface/midi_surface
)
