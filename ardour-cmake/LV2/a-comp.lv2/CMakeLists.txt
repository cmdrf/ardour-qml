add_library(a-comp SHARED
	${L}/plugins/a-comp.lv2/a-comp.c
)

target_link_libraries(a-comp PUBLIC
	PkgConfig::LV2
)

configure_file(${L}/plugins/a-comp.lv2/a-comp.ttl.in a-comp.ttl)
configure_file("${L}/plugins/a-comp.lv2/a-comp#stereo.ttl.in" "a-comp#stereo.ttl")
configure_file(${L}/plugins/a-comp.lv2/manifest.ttl.in manifest.ttl)
configure_file(${L}/plugins/a-comp.lv2/presets.ttl.in presets.ttl)
