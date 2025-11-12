set(A ardour)

find_package(Boost REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(GLIB2 REQUIRED glib-2.0 glibmm-2.4)

add_library(ardourlib
#    ${A}/libs/ardour/aarch64_neon_functions.cc
    ${A}/libs/ardour/amp.cc
#    ${A}/libs/ardour/analyser.cc
#    ${A}/libs/ardour/analysis_graph.cc
    ${A}/libs/ardour/arm_neon_functions.cc
    ${A}/libs/ardour/async_midi_port.cc
    ${A}/libs/ardour/audioanalyser.cc
    ${A}/libs/ardour/audio_backend.cc
    ${A}/libs/ardour/audio_buffer.cc
    ${A}/libs/ardour/audioengine.cc

    ${A}/libs/ardour/globals.cc
    ${A}/libs/ardour/graph.cc
    ${A}/libs/ardour/graph_edges.cc
    ${A}/libs/ardour/graphnode.cc
    ${A}/libs/ardour/iec1ppmdsp.cc
    ${A}/libs/ardour/iec2ppmdsp.cc
    ${A}/libs/ardour/import.cc
    ${A}/libs/ardour/import_pt.cc
    ${A}/libs/ardour/instrument_info.cc
    ${A}/libs/ardour/internal_return.cc
    ${A}/libs/ardour/internal_send.cc
    ${A}/libs/ardour/io.cc
    ${A}/libs/ardour/io_plug.cc
    ${A}/libs/ardour/io_processor.cc
    ${A}/libs/ardour/io_tasklist.cc
    ${A}/libs/ardour/kmeterdsp.cc
    ${A}/libs/ardour/ladspa_plugin.cc
    ${A}/libs/ardour/latent.cc
    ${A}/libs/ardour/legatize.cc
    ${A}/libs/ardour/library.cc
    ${A}/libs/ardour/linux_vst_support.cc
    ${A}/libs/ardour/load-save-session-collection.sh
    ${A}/libs/ardour/load-save-session.sh
    ${A}/libs/ardour/location.cc
    ${A}/libs/ardour/location_importer.cc
    ${A}/libs/ardour/ltc_file_reader.cc
    ${A}/libs/ardour/ltc_slave.cc
#    ${A}/libs/ardour/lua_api.cc
#    ${A}/libs/ardour/luabindings.cc
#    ${A}/libs/ardour/luaproc.cc
#    ${A}/libs/ardour/luascripting.cc
    ${A}/libs/ardour/lufs_meter.cc
    ${A}/libs/ardour/lv2_evbuf.cc
    ${A}/libs/ardour/lv2_plugin.cc
    ${A}/libs/ardour/lxvst_plugin.cc
#    ${A}/libs/ardour/mac_vst_plugin.cc
#    ${A}/libs/ardour/mac_vst_support.cc
    ${A}/libs/ardour/meter.cc
    ${A}/libs/ardour/midi_automation_list_binder.cc
    ${A}/libs/ardour/midi_buffer.cc
    ${A}/libs/ardour/midi_channel_filter.cc
    ${A}/libs/ardour/midi_clock_slave.cc
    ${A}/libs/ardour/mididm.cc
    ${A}/libs/ardour/midi_model.cc
    ${A}/libs/ardour/midi_patch_manager.cc
    ${A}/libs/ardour/midi_playlist.cc
    ${A}/libs/ardour/midi_port.cc
    ${A}/libs/ardour/midiport_manager.cc
    ${A}/libs/ardour/midi_region.cc
    ${A}/libs/ardour/midi_ring_buffer.cc
    ${A}/libs/ardour/midi_scene_change.cc
    ${A}/libs/ardour/midi_scene_changer.cc
    ${A}/libs/ardour/midi_source.cc
    ${A}/libs/ardour/midi_state_tracker.cc
    ${A}/libs/ardour/midi_stretch.cc
    ${A}/libs/ardour/midi_track.cc
    ${A}/libs/ardour/midi_ui.cc
    ${A}/libs/ardour/minibpm.cc
    ${A}/libs/ardour/mix.cc

    ${A}/libs/ardour/send.cc
    ${A}/libs/ardour/session_bundles.cc
    ${A}/libs/ardour/session_butler.cc
    ${A}/libs/ardour/session.cc
)

target_include_directories(ardourlib PUBLIC
    ${GLIB2_INCLUDE_DIRS}
    /usr/include/sigc++-2.0/
    /usr/lib/x86_64-linux-gnu/sigc++-2.0/include/
    /usr/include/lilv-0/
    ${A}/libs/pbd
    ${A}/libs/temporal
    ${A}/libs/ptformat
    ${A}/libs/ardour
    ${A}/libs/evoral
    ${A}/libs/midi++2
    ${A}/libs/zita-resampler
    ${A}/libs/ctrl-interface/control_protocol
    ${A}/libs/lua
    ${A}/libs/audiographer
    ${A}/libs/libltc/ltc/
)

target_link_libraries(ardourlib PUBLIC
#    Boost::temporal
    LibXml2::LibXml2
    ${GLIB2_LIBRARIES}
)

target_compile_definitions(ardourlib PRIVATE
    PROGRAM_NAME="Ardour"
    PROGRAM_VERSION="6"
    PACKAGE="Ardour"
    CURRENT_SESSION_FILE_VERSION=7003
)
