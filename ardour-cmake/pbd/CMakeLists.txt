add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/signals_generated.h
	COMMAND python3 ${L}/pbd/pbd/signals.py ${CMAKE_CURRENT_BINARY_DIR}/signals_generated.h
)

configure_file(libpbd-config.h.in libpbd-config.h)

add_library(pbd
	${L}/pbd/base_ui.cc
	${L}/pbd/basename.cc
	${L}/pbd/cartesian.cc
	${L}/pbd/ccurl.cc
	${L}/pbd/command.cc
	${L}/pbd/configuration_variable.cc
	${L}/pbd/convert.cc
	${L}/pbd/cpus.cc
	${L}/pbd/debug.cc
	${L}/pbd/epa.cc
	${L}/pbd/event_loop.cc
	${L}/pbd/history_owner.cc
	${L}/pbd/pool.cc
	${L}/pbd/progress.cc
	${L}/pbd/property_list.cc
	${L}/pbd/md5.cc
#	${L}/pbd/boost_debug.cc
	${L}/pbd/pathexpand.cc
	${L}/pbd/system_exec.cc
	${L}/pbd/xml++.cc
	${L}/pbd/stateful.cc
	${L}/pbd/whitespace.cc
	${L}/pbd/fpu.cc
#    ${L}/pbd/msvc/msvc_pbd.cc
#    ${L}/pbd/msvc/mountpoint.cc
	${L}/pbd/locale_guard.cc
	${L}/pbd/spinlock.cc
	${L}/pbd/uuid.cc
	${L}/pbd/strsplit.cc
	${L}/pbd/microseconds.cc
	${L}/pbd/timing.cc
	${L}/pbd/crossthread.cc
	${L}/pbd/pbd.cc
	${L}/pbd/textreceiver.cc
	${L}/pbd/stateful_diff_command.cc
	${L}/pbd/tlsf.cc
	${L}/pbd/undo.cc
	${L}/pbd/transmitter.cc
	${L}/pbd/file_archive.cc
	${L}/pbd/file_utils.cc
	${L}/pbd/string_convert.cc
	${L}/pbd/utf8_utils.cc
	${L}/pbd/inflater.cc
#    ${L}/pbd/windows_special_dirs.cc
	${L}/pbd/semutils.cc
	${L}/pbd/strreplace.cc
#    ${L}/pbd/windows_timer_utils.cc
	${L}/pbd/reallocpool.cc
	${L}/pbd/glib_event_source.cc
	${L}/pbd/localtime_r.cc
	${L}/pbd/pcg_rand.cc
	${L}/pbd/timer.cc
	${L}/pbd/resource.cc
	${L}/pbd/receiver.cc
	${L}/pbd/mountpoint.cc
	${L}/pbd/openuri.cc
	${L}/pbd/signals.cc
	${L}/pbd/enums.cc
	${L}/pbd/pthread_utils.cc
	${L}/pbd/enumwriter.cc
	${L}/pbd/demangle.cc
	${L}/pbd/stacktrace.cc
	${L}/pbd/ffs.cc
#    ${L}/pbd/windows_mmcss.cc
	${L}/pbd/downloader.cc
	${L}/pbd/shortpath.cc
	${L}/pbd/controllable.cc
	${L}/pbd/malign.cc
	${L}/pbd/search_path.cc
	${L}/pbd/id.cc
	${L}/pbd/error.cc

	${CMAKE_CURRENT_BINARY_DIR}/signals_generated.h
)

if(APPLE)
	target_sources(pbd PUBLIC ${L}/pbd/cocoa_open_uri.mm)

	target_link_libraries(pbd PUBLIC
		"$<LINK_LIBRARY:FRAMEWORK,Cocoa>"
	)
endif()

target_include_directories(pbd PUBLIC
	${Boost_INCLUDE_DIRS}
	${CMAKE_CURRENT_BINARY_DIR}/..
	${L}/pbd/
)

target_include_directories(pbd PRIVATE
	${CMAKE_CURRENT_BINARY_DIR} # For libpbd-config.h
)

target_link_libraries(pbd PUBLIC
	PkgConfig::GLIB2
)
