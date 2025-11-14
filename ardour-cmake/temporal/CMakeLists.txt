add_library(temporal SHARED
	${L}/temporal/bbt_time.cc
	${L}/temporal/beats.cc
	${L}/temporal/debug.cc
	${L}/temporal/domain.cc
	${L}/temporal/enums.cc
	${L}/temporal/range.cc
	${L}/temporal/superclock.cc
	${L}/temporal/tempo.cc
	${L}/temporal/time.cc
	${L}/temporal/timeline.cc
)

target_include_directories(temporal PUBLIC
	${L}/temporal
)

target_link_libraries(temporal PUBLIC
	pbd
)
