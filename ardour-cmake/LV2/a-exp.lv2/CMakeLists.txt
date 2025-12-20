add_library(a-exp SHARED
	${L}/plugins/a-exp.lv2/a-exp.c
)

target_link_libraries(a-exp PUBLIC
	PkgConfig::LV2
)

configure_file(${L}/plugins/a-exp.lv2/a-exp.ttl.in a-exp.ttl)
configure_file(${L}/plugins/a-exp.lv2/manifest.ttl.in manifest.ttl)
configure_file("${L}/plugins/a-exp.lv2/a-exp#stereo.ttl.in" "a-exp#stereo.ttl")
