add_library(a-eq SHARED
	${L}/plugins/a-eq.lv2/a-eq.c
)

target_link_libraries(a-eq PUBLIC
	PkgConfig::LV2
)

configure_file(${L}/plugins/a-eq.lv2/a-eq.ttl.in a-eq.ttl)
configure_file(${L}/plugins/a-eq.lv2/manifest.ttl.in manifest.ttl)
