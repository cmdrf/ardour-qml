if(DEBUG)
	add_library(a-vapor SHARED
		${L}/plugins/a-vapor.lv2/a-vapor.c
	)

	target_link_libraries(a-vapor PUBLIC
		PkgConfig::LV2
	)

	configure_file(${L}/plugins/a-vapor.lv2/a-vapor.ttl.in a-vapor.ttl)
	configure_file(${L}/plugins/a-vapor.lv2/manifest.ttl.in manifest.ttl)
endif()
