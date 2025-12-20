add_library(a-reverb SHARED
	${L}/plugins/a-reverb.lv2/a-reverb.c
)

target_link_libraries(a-reverb PUBLIC
	PkgConfig::LV2
)

configure_file(${L}/plugins/a-reverb.lv2/a-reverb.ttl.in a-reverb.ttl)
configure_file(${L}/plugins/a-reverb.lv2/manifest.ttl.in manifest.ttl)
