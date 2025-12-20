add_library(a-fluidsynth SHARED
	${L}/plugins/a-fluidsynth.lv2/a-fluidsynth.cc
)

target_link_libraries(a-fluidsynth PUBLIC
	PkgConfig::LV2
	fluidsynth
)

configure_file(${L}/plugins/a-fluidsynth.lv2/a-fluidsynth.ttl.in a-fluidsynth.ttl)
configure_file(${L}/plugins/a-fluidsynth.lv2/manifest.ttl.in manifest.ttl)
