add_library(zita-resampler STATIC
	${L}/zita-resampler/cresampler.cc
	${L}/zita-resampler/resampler-table.cc
	${L}/zita-resampler/resampler.cc
	${L}/zita-resampler/vmresampler.cc
	${L}/zita-resampler/vresampler.cc
)

target_include_directories(zita-resampler PUBLIC
	${L}/zita-resampler
)

set_property(TARGET zita-resampler PROPERTY POSITION_INDEPENDENT_CODE ON)
