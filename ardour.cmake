set(L ${CMAKE_CURRENT_SOURCE_DIR}/ardour/libs)
set(A ${L}/ardour)

find_package(Boost REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(LibArchive REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(GLIB2 REQUIRED IMPORTED_TARGET glib-2.0 glibmm-2.4)

find_path(LILV_INCLUDE_DIR
    NAMES lilv/lilv.h
    HINTS /opt/homebrew/include/lilv-0
    REQUIRED
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pbd/signals_generated.h
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/pbd/
    COMMAND python3 ${L}/pbd/pbd/signals.py ${CMAKE_CURRENT_BINARY_DIR}/pbd/signals_generated.h
)

add_library(ardourlib
#    ${A}/aarch64_neon_functions.cc
    ${A}/amp.cc
#    ${A}/analyser.cc
#    ${A}/analysis_graph.cc
    ${A}/arm_neon_functions.cc
    ${A}/async_midi_port.cc
    ${A}/audioanalyser.cc
    ${A}/audio_backend.cc
    ${A}/audio_buffer.cc
    ${A}/audioengine.cc

    ${A}/globals.cc
    ${A}/graph.cc
    ${A}/graph_edges.cc
    ${A}/graphnode.cc
    ${A}/iec1ppmdsp.cc
    ${A}/iec2ppmdsp.cc
    ${A}/import.cc
    ${A}/import_pt.cc
    ${A}/instrument_info.cc
    ${A}/internal_return.cc
    ${A}/internal_send.cc
    ${A}/io.cc
    ${A}/io_plug.cc
    ${A}/io_processor.cc
    ${A}/io_tasklist.cc
    ${A}/kmeterdsp.cc
    ${A}/ladspa_plugin.cc
    ${A}/latent.cc
    ${A}/legatize.cc
    ${A}/library.cc
    ${A}/linux_vst_support.cc
    ${A}/load-save-session-collection.sh
    ${A}/load-save-session.sh
    ${A}/location.cc
    ${A}/location_importer.cc
    ${A}/ltc_file_reader.cc
    ${A}/ltc_slave.cc
#    ${A}/lua_api.cc
#    ${A}/luabindings.cc
#    ${A}/luaproc.cc
#    ${A}/luascripting.cc
    ${A}/lufs_meter.cc
    ${A}/lv2_evbuf.cc
    ${A}/lv2_plugin.cc
    ${A}/lxvst_plugin.cc
#    ${A}/mac_vst_plugin.cc
#    ${A}/mac_vst_support.cc
    ${A}/meter.cc
    ${A}/midi_automation_list_binder.cc
    ${A}/midi_buffer.cc
    ${A}/midi_channel_filter.cc
    ${A}/midi_clock_slave.cc
    ${A}/mididm.cc
    ${A}/midi_model.cc
    ${A}/midi_patch_manager.cc
    ${A}/midi_playlist.cc
    ${A}/midi_port.cc
    ${A}/midiport_manager.cc
    ${A}/midi_region.cc
    ${A}/midi_ring_buffer.cc
    ${A}/midi_scene_change.cc
    ${A}/midi_scene_changer.cc
    ${A}/midi_source.cc
    ${A}/midi_state_tracker.cc
    ${A}/midi_stretch.cc
    ${A}/midi_track.cc
    ${A}/midi_ui.cc
    ${A}/minibpm.cc
    ${A}/mix.cc

    ${A}/send.cc
    ${A}/session_bundles.cc
    ${A}/session_butler.cc
    ${A}/session.cc

    ${L}/lua/lua.cc

    ${CMAKE_CURRENT_BINARY_DIR}/pbd/signals_generated.h
)

target_include_directories(ardourlib PUBLIC
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${LILV_INCLUDE_DIR}
    /usr/include/sigc++-2.0/
    /usr/lib/x86_64-linux-gnu/sigc++-2.0/include/
    ${L}/pbd
    ${L}/temporal
    ${L}/ptformat
    ${L}/ardour
    ${L}/evoral
    ${L}/midi++2
    ${L}/zita-resampler
    ${L}/ctrl-interface/control_protocol
    ${L}/lua
    ${L}/audiographer
    ${L}/libltc/ltc/
)

target_link_libraries(ardourlib PUBLIC
    LibXml2::LibXml2
    LibArchive::LibArchive
    PkgConfig::GLIB2
)

target_compile_definitions(ardourlib PRIVATE
    PROGRAM_NAME="Ardour"
    PROGRAM_VERSION="6"
    PACKAGE="Ardour"
    CURRENT_SESSION_FILE_VERSION=7003
    HAVE_LV2_1_10_0
)
