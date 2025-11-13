add_library(audiographer
	${L}/audiographer/src/debug_utils.cc
	${L}/audiographer/src/general/analyser.cc
	${L}/audiographer/src/general/broadcast_info.cc
	${L}/audiographer/src/general/demo_noise.cc
	${L}/audiographer/src/general/limiter.cc
	${L}/audiographer/src/general/loudness_reader.cc
	${L}/audiographer/src/general/normalizer.cc
	${L}/audiographer/src/general/sample_format_converter.cc
	${L}/audiographer/src/general/sndfile.cc
	${L}/audiographer/src/general/sr_converter.cc
	${L}/audiographer/src/routines.cc
	${L}/audiographer/private/limiter/limiter.cc
	${L}/audiographer/private/gdither/gdither.cc
)

target_include_directories(audiographer PUBLIC
	${L}/audiographer
	${Boost_INCLUDE_DIRS}
	${A} # Circular dependency, yay!
)

target_link_libraries(audiographer PUBLIC
	PkgConfig::GLIB2
	pbd
	temporal
)
