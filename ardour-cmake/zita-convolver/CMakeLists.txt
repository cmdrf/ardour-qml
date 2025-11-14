pkg_check_modules(FFTW3 REQUIRED IMPORTED_TARGET fftw3f)

add_library(zita-convolver STATIC
	${L}/zita-convolver/zita-convolver.cc
)

target_include_directories(zita-convolver PUBLIC
	${L}/zita-convolver
)

target_link_libraries(zita-convolver PUBLIC
	PkgConfig::FFTW3
)

set_property(TARGET zita-convolver PROPERTY POSITION_INDEPENDENT_CODE ON)
