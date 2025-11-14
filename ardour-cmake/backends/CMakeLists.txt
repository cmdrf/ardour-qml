add_library(dummy_audiobackend SHARED
	${L}/backends/dummy/dummy_audiobackend.cc
)

target_link_libraries(dummy_audiobackend PUBLIC
	PkgConfig::GLIB2
	PkgConfig::LTC
	pbd
	ardourlib
)
