add_library(reasonablesynth SHARED
	${L}/plugins/reasonablesynth.lv2/lv2.c
	${L}/plugins/reasonablesynth.lv2/rsynth.c
)

target_link_libraries(reasonablesynth PUBLIC
	PkgConfig::LV2
)

configure_file(${L}/plugins/reasonablesynth.lv2/reasonablesynth.ttl.in reasonablesynth.ttl)
configure_file(${L}/plugins/reasonablesynth.lv2/manifest.ttl.in manifest.ttl)
