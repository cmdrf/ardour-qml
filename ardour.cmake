set(L ${CMAKE_CURRENT_SOURCE_DIR}/ardour/libs)
set(A ${L}/ardour)

find_package(Boost REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(LibArchive REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(GLIB2 REQUIRED IMPORTED_TARGET glib-2.0 glibmm-2.4)
pkg_check_modules(LTC REQUIRED IMPORTED_TARGET ltc)
pkg_check_modules(SNDFILE REQUIRED IMPORTED_TARGET sndfile)

find_path(LILV_INCLUDE_DIR
    NAMES lilv/lilv.h
    HINTS /opt/homebrew/include/lilv-0
    REQUIRED
)
find_library(LILV_LIBRARY NAMES lilv-0 REQUIRED)
find_library(RUBBERBAND_LIBRARY NAMES rubberband REQUIRED)
find_library(LO_LIBRARY NAMES lo REQUIRED)

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
    ${A}/lua_api.cc
    ${A}/luabindings.cc
    ${A}/luaproc.cc
    ${A}/luascripting.cc
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
    ${A}/mixer_scene.cc
    ${A}/mode.cc
    ${A}/monitor_control.cc
    ${A}/monitor_port.cc
    ${A}/monitor_processor.cc
    ${A}/monitor_return.cc
    ${A}/mp3fileimportable.cc
    ${A}/mp3filesource.cc
    ${A}/mtc_slave.cc
    ${A}/mtdm.cc
    ${A}/mute_control.cc
    ${A}/mute_master.cc
    ${A}/muteable.cc
    ${A}/onset_detector.cc
    ${A}/operations.cc
    ${A}/pan_controllable.cc
    ${A}/pannable.cc
    ${A}/panner_manager.cc
    ${A}/panner_shell.cc
    ${A}/panner.cc
    ${A}/parameter_descriptor.cc
    ${A}/phase_control.cc
    ${A}/playlist_factory.cc
    ${A}/playlist_source.cc
    ${A}/playlist.cc
    ${A}/plug_insert_base.cc
    ${A}/plugin_insert.cc
    ${A}/plugin_manager.cc
    ${A}/plugin_scan_result.cc
    ${A}/plugin.cc
    ${A}/polarity_processor.cc
    ${A}/port_engine_shared.cc
    ${A}/port_insert.cc
    ${A}/port_manager.cc
    ${A}/port_set.cc
    ${A}/port.cc
    ${A}/presentation_info.cc
    ${A}/process_thread.cc
    ${A}/processor.cc
    ${A}/quantize.cc
    ${A}/raw_midi_parser.cc
    ${A}/rb_effect.cc
    ${A}/rc_configuration.cc
    ${A}/readable.cc
    ${A}/readonly_control.cc
    ${A}/recent_sessions.cc
    ${A}/record_enable_control.cc
    ${A}/record_safe_control.cc
    ${A}/region_factory.cc
    ${A}/region_fx_plugin.cc
    ${A}/region.cc
    ${A}/resampled_source.cc
    ${A}/return.cc
    ${A}/reverse.cc
    ${A}/route_group_member.cc
    ${A}/route_group.cc
    ${A}/route.cc
    ${A}/rt_midibuffer.cc
    ${A}/rt_task.cc
    ${A}/rt_tasklist.cc
    ${A}/scene_change.cc
    ${A}/search_paths.cc
    ${A}/segment_descriptor.cc
    ${A}/selection.cc
    ${A}/send.cc
    ${A}/session_bundles.cc
    ${A}/session_butler.cc
    ${A}/session.cc
    ${A}/sha1.c
    ${A}/sidechain.cc
    ${A}/simple_export.cc
    ${A}/slavable_automation_control.cc
    ${A}/slavable.cc
    ${A}/smf_source.cc
    ${A}/sndfile_helpers.cc
    ${A}/sndfileimportable.cc
    ${A}/sndfilesource.cc
    ${A}/solo_control.cc
    ${A}/solo_isolate_control.cc
    ${A}/solo_mute_release.cc
    ${A}/solo_safe_control.cc
    ${A}/soundcloud_upload.cc
    ${A}/source_factory.cc
    ${A}/source.cc
    ${A}/speakers.cc
    ${A}/srcfilesource.cc
#   ${A}/sse_avx_functions_64bit_win.s
#   ${A}/sse_functions_64bit_win.s
#    ${A}/sse_functions_64bit.s
#    ${A}/sse_functions_avx_linux.cc
#    ${A}/sse_functions_avx.cc
#    ${A}/sse_functions_xmm.cc
#    ${A}/sse_functions.s
#    ${A}/st_pitch.cc
#    ${A}/st_stretch.cc
#    ${A}/step_sequencer.cc
    ${A}/strip_silence.cc
    ${A}/stripable.cc
    ${A}/surround_pannable.cc
    ${A}/surround_return.cc
    ${A}/surround_send.cc
    ${A}/system_exec.cc
    ${A}/tailtime.cc
    ${A}/template_utils.cc
    ${A}/tempo_map_importer.cc
    ${A}/thawlist.cc
    ${A}/thread_buffers.cc
    ${A}/ticker.cc
    ${A}/track.cc
    ${A}/transform.cc
    ${A}/transient_detector.cc
    ${A}/transport_fsm.cc
    ${A}/transport_master_manager.cc
    ${A}/transport_master.cc
    ${A}/transpose.cc
    ${A}/triggerbox.cc
    ${A}/unknown_processor.cc
    ${A}/uri_map.cc
    ${A}/user_bundle.cc
    ${A}/utils.cc
    ${A}/vca_manager.cc
    ${A}/vca.cc
    ${A}/velocity_control.cc
    ${A}/video_tools_paths.cc
#    ${A}/vst_plugin.cc
#    ${A}/vst_state.cc
#    ${A}/vst2_scan.cc
#    ${A}/vst3_host.cc
#    ${A}/vst3_module.cc
#    ${A}/vst3_plugin.cc
#    ${A}/vst3_scan.cc
    ${A}/vumeterdsp.cc
#   ${A}/windows_vst_plugin.cc
    ${A}/worker.cc
#    ${A}/x86_functions_avx512f.cc
#    ${A}/x86_functions_fma.cc
    ${A}/zeroconf.cc

    ${L}/lua/lua.cc
    ${L}/zita-convolver/zita-convolver.cc


    ${L}/fluidsynth/src/fluid_adsr_env.c
    ${L}/fluidsynth/src/fluid_adsr_env.h
    ${L}/fluidsynth/src/fluid_chan.c
    ${L}/fluidsynth/src/fluid_chan.h
    ${L}/fluidsynth/src/fluid_chorus.c
    ${L}/fluidsynth/src/fluid_chorus.h
    ${L}/fluidsynth/src/fluid_conv_tables.h
    ${L}/fluidsynth/src/fluid_conv_tables.inc.h
    ${L}/fluidsynth/src/fluid_conv.c
    ${L}/fluidsynth/src/fluid_conv.h
    ${L}/fluidsynth/src/fluid_defsfont.c
    ${L}/fluidsynth/src/fluid_defsfont.h
    ${L}/fluidsynth/src/fluid_event.c
    ${L}/fluidsynth/src/fluid_event.h
    ${L}/fluidsynth/src/fluid_gen.c
    ${L}/fluidsynth/src/fluid_gen.h
    ${L}/fluidsynth/src/fluid_hash.c
    ${L}/fluidsynth/src/fluid_hash.h
    ${L}/fluidsynth/src/fluid_iir_filter.c
    ${L}/fluidsynth/src/fluid_iir_filter.h
    ${L}/fluidsynth/src/fluid_lfo.c
    ${L}/fluidsynth/src/fluid_lfo.h
    ${L}/fluidsynth/src/fluid_list.c
    ${L}/fluidsynth/src/fluid_list.h
    ${L}/fluidsynth/src/fluid_midi.c
    ${L}/fluidsynth/src/fluid_midi.h
    ${L}/fluidsynth/src/fluid_mod.c
    ${L}/fluidsynth/src/fluid_mod.h
    ${L}/fluidsynth/src/fluid_phase.h
    ${L}/fluidsynth/src/fluid_rev.c
    ${L}/fluidsynth/src/fluid_rev.h
    ${L}/fluidsynth/src/fluid_ringbuffer.c
    ${L}/fluidsynth/src/fluid_ringbuffer.h
    ${L}/fluidsynth/src/fluid_rvoice_dsp_tables.inc.h
    ${L}/fluidsynth/src/fluid_rvoice_dsp.c
    ${L}/fluidsynth/src/fluid_rvoice_event.c
    ${L}/fluidsynth/src/fluid_rvoice_event.h
    ${L}/fluidsynth/src/fluid_rvoice_mixer.c
    ${L}/fluidsynth/src/fluid_rvoice_mixer.h
    ${L}/fluidsynth/src/fluid_rvoice.c
    ${L}/fluidsynth/src/fluid_rvoice.h
    ${L}/fluidsynth/src/fluid_samplecache.c
    ${L}/fluidsynth/src/fluid_samplecache.h
    ${L}/fluidsynth/src/fluid_settings.c
    ${L}/fluidsynth/src/fluid_settings.h
    ${L}/fluidsynth/src/fluid_sffile.c
    ${L}/fluidsynth/src/fluid_sffile.h
    ${L}/fluidsynth/src/fluid_sfont.c
    ${L}/fluidsynth/src/fluid_sfont.h
    ${L}/fluidsynth/src/fluid_synth_monopoly.c
    ${L}/fluidsynth/src/fluid_synth.c
    ${L}/fluidsynth/src/fluid_synth.h
    ${L}/fluidsynth/src/fluid_sys.c
    ${L}/fluidsynth/src/fluid_sys.h
    ${L}/fluidsynth/src/fluid_tuning.c
    ${L}/fluidsynth/src/fluid_tuning.h
    ${L}/fluidsynth/src/fluid_voice.c
    ${L}/fluidsynth/src/fluid_voice.h
    ${L}/fluidsynth/src/fluidsynth_priv.h

    ${L}/pbd/pool.cc
    ${L}/pbd/configuration_variable.cc
    ${L}/pbd/progress.cc
    ${L}/pbd/property_list.cc
    ${L}/pbd/md5.cc
#    ${L}/pbd/boost_debug.cc
    ${L}/pbd/pathexpand.cc
    ${L}/pbd/system_exec.cc
    ${L}/pbd/xml++.cc
    ${L}/pbd/stateful.cc
    ${L}/pbd/whitespace.cc
    ${L}/pbd/fpu.cc
#    ${L}/pbd/msvc/msvc_pbd.cc
#    ${L}/pbd/msvc/mountpoint.cc
    ${L}/pbd/base_ui.cc
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
    ${L}/pbd/epa.cc
    ${L}/pbd/tlsf.cc
    ${L}/pbd/event_loop.cc
    ${L}/pbd/basename.cc
    ${L}/pbd/undo.cc
    ${L}/pbd/transmitter.cc
    ${L}/pbd/file_archive.cc
    ${L}/pbd/file_utils.cc
    ${L}/pbd/string_convert.cc
    ${L}/pbd/pbd/abstract_ui.cc
    ${L}/pbd/crossthread.posix.cc
    ${L}/pbd/utf8_utils.cc
    ${L}/pbd/inflater.cc
    ${L}/pbd/cartesian.cc
    ${L}/pbd/debug.cc
    ${L}/pbd/cpus.cc
    ${L}/pbd/convert.cc
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
    ${L}/pbd/crossthread.win.cc
    ${L}/pbd/shortpath.cc
    ${L}/pbd/controllable.cc
    ${L}/pbd/malign.cc
    ${L}/pbd/search_path.cc
    ${L}/pbd/command.cc
    ${L}/pbd/id.cc
    ${L}/pbd/error.cc

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
    ${L}/zita-convolver
    ${L}/fluidsynth/fluidsynth
    ${L}/appleutility/CoreAudio105/
#    ${L}
#    ${L}/vst3
)

target_link_libraries(ardourlib PUBLIC
    LibXml2::LibXml2
    LibArchive::LibArchive
    PkgConfig::GLIB2
    PkgConfig::LTC
    PkgConfig::SNDFILE
    ${RUBBERBAND_LIBRARY}
    ${LO_LIBRARY}
    ${LILV_LIBRARY}
)

if(APPLE)
    target_link_libraries(ardourlib PUBLIC
        "$<LINK_LIBRARY:FRAMEWORK,AudioToolbox>"
        "$<LINK_LIBRARY:FRAMEWORK,CoreAudio>"
    )
endif()

target_compile_definitions(ardourlib PRIVATE
    PROGRAM_NAME="Ardour"
    PROGRAM_VERSION="6"
    PACKAGE="Ardour"
    CURRENT_SESSION_FILE_VERSION=7003
    HAVE_LV2_1_10_0
    USE_RUBBERBAND
#    HAVE_SOUNDTOUCH
)
