add_library(a-delay SHARED
	${L}/plugins/a-delay.lv2/a-delay.c
)

target_link_libraries(a-delay PUBLIC
	PkgConfig::LV2
)

configure_file(${L}/plugins/a-delay.lv2/a-delay.ttl.in a-delay.ttl)
configure_file(${L}/plugins/a-delay.lv2/manifest.ttl.in manifest.ttl)
configure_file(${L}/plugins/a-delay.lv2/presets.ttl.in presets.ttl)
