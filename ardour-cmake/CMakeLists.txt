set(L ${CMAKE_SOURCE_DIR}/ardour/libs)
set(A ${L}/ardour)

find_package(Boost REQUIRED)
find_package(CURL REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(LibArchive REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(GLIB2 REQUIRED IMPORTED_TARGET glib-2.0 glibmm-2.4 giomm-2.4)
pkg_check_modules(LTC REQUIRED IMPORTED_TARGET ltc)
pkg_check_modules(SNDFILE REQUIRED IMPORTED_TARGET sndfile)
pkg_check_modules(TAGLIB REQUIRED IMPORTED_TARGET taglib)
pkg_check_modules(SIGCPP REQUIRED IMPORTED_TARGET sigc++-2.0)
pkg_check_modules(VAMP REQUIRED IMPORTED_TARGET vamp-hostsdk)
pkg_check_modules(SAMPLERATE REQUIRED IMPORTED_TARGET samplerate)
pkg_check_modules(RUBBERBAND REQUIRED IMPORTED_TARGET rubberband)
pkg_check_modules(LILV REQUIRED IMPORTED_TARGET lilv-0)
pkg_check_modules(LO REQUIRED IMPORTED_TARGET liblo)

add_compile_definitions(
	PROGRAM_NAME="Ardour"
	PROGRAM_VERSION="6"
	PACKAGE="Ardour"
	CURRENT_SESSION_FILE_VERSION=7003
	HAVE_LV2_1_10_0
	USE_RUBBERBAND
#    HAVE_SOUNDTOUCH
)

if(APPLE)
	add_compile_definitions(
		HAVE_SYS_STATVFS_H
	)
endif()

add_subdirectory(audiographer)
add_subdirectory(ctrl-interface)
add_subdirectory(evoral)
add_subdirectory(fluidsynth)
add_subdirectory(lua)
add_subdirectory(midi++2)
add_subdirectory(pbd)
add_subdirectory(temporal)
add_subdirectory(zita-convolver)
add_subdirectory(zita-resampler)

if(APPLE)
	add_subdirectory(appleutility)
endif()

configure_file(revision.cc.in revision.cc)

add_library(ardourlib
	${CMAKE_CURRENT_BINARY_DIR}/revision.cc

#    ${A}/aarch64_neon_functions.cc
    ${A}/amp.cc
	${A}/analyser.cc
	${A}/analysis_graph.cc
    ${A}/arm_neon_functions.cc
    ${A}/async_midi_port.cc
    ${A}/audioanalyser.cc
    ${A}/audio_backend.cc
    ${A}/audio_buffer.cc
	${A}/audio_library.cc
	${A}/audio_playlist_importer.cc
	${A}/audio_playlist_source.cc
	${A}/audio_playlist.cc
	${A}/audio_port.cc
	${A}/audio_region_importer.cc
	${A}/audio_track_importer.cc
	${A}/audio_track.cc
    ${A}/audioengine.cc
	${A}/audio_unit.cc
	${A}/audioanalyser.cc
	${A}/audiofile_tagger.cc
	${A}/audiofilesource.cc
	${A}/audioregion.cc
	${A}/audiorom.cc
	${A}/audiosource.cc
	${A}/auditioner.cc
	${A}/automatable.cc
	${A}/automation_control.cc
	${A}/automation_list.cc
	${A}/automation_watch.cc
	${A}/automation.cc
	${A}/auv2_scan.cc
#	${A}/beatbox.cc
	${A}/broadcast_info.cc
	${A}/buffer_manager.cc
	${A}/buffer_set.cc
	${A}/buffer.cc
	${A}/bundle.cc
	${A}/butler.cc
	${A}/caimportable.cc
	${A}/capturing_processor.cc
	${A}/chan_count.cc
	${A}/chan_mapping.cc
	${A}/circular_buffer.cc
	${A}/clip_library.cc
	${A}/control_group.cc
	${A}/control_protocol_manager.cc
	${A}/convolver.cc
	${A}/coreaudiosource.cc
	${A}/cycle_timer.cc
	${A}/data_type.cc
	${A}/debug.cc
	${A}/default_click.cc
	${A}/delayline.cc
	${A}/delivery.cc
	${A}/directory_names.cc
	${A}/disk_io.cc
	${A}/disk_reader.cc
	${A}/disk_writer.cc
	${A}/dsp_filter.cc
	${A}/ebur128_analysis.cc
	${A}/element_import_handler.cc
	${A}/element_importer.cc
	${A}/engine_slave.cc
	${A}/enums.cc
	${A}/event_type_map.cc
	${A}/export_channel_configuration.cc
	${A}/export_channel.cc
	${A}/export_failed.cc
	${A}/export_filename.cc
	${A}/export_format_base.cc
	${A}/export_format_manager.cc
	${A}/export_format_specification.cc
	${A}/export_formats.cc
	${A}/export_graph_builder.cc
	${A}/export_handler.cc
	${A}/export_preset.cc
	${A}/export_profile_manager.cc
	${A}/export_smf_writer.cc
	${A}/export_status.cc
	${A}/export_timespan.cc
	${A}/ffmpegfileimportable.cc
	${A}/ffmpegfilesource.cc
	${A}/file_source.cc
	${A}/filename_extensions.cc
	${A}/filesystem_paths.cc
	${A}/filter.cc
	${A}/find_session.cc
	${A}/fixed_delay.cc
	${A}/fluid_synth.cc
	${A}/gain_control.cc
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
	${A}/session_click.cc
	${A}/session_command.cc
	${A}/session_configuration.cc
	${A}/session_directory.cc
	${A}/session_events.cc
	${A}/session_export.cc
	${A}/session_handle.cc
	${A}/session_ltc.cc
	${A}/session_metadata.cc
	${A}/session_midi.cc
	${A}/session_object.cc
	${A}/session_playlists.cc
	${A}/session_process.cc
	${A}/session_rtevents.cc
	${A}/session_state_utils.cc
	${A}/session_state.cc
	${A}/session_time.cc
	${A}/session_transport.cc
	${A}/session_vst.cc
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
)

target_include_directories(ardourlib PUBLIC
    ${Boost_INCLUDE_DIRS}
	${CMAKE_CURRENT_BINARY_DIR}
    ${L}/ptformat
    ${L}/ardour
    ${L}/ctrl-interface/control_protocol
    ${L}/libltc/ltc/
#    ${L}
#    ${L}/vst3
)

target_link_libraries(ardourlib PUBLIC
	CURL::libcurl
    LibXml2::LibXml2
    LibArchive::LibArchive
    PkgConfig::GLIB2
    PkgConfig::LTC
    PkgConfig::SNDFILE
	PkgConfig::TAGLIB
	PkgConfig::VAMP
	PkgConfig::SAMPLERATE
	PkgConfig::RUBBERBAND
	PkgConfig::LILV
	PkgConfig::LO
	audiographer
	evoral
	fluidsynth
	lua
	midi++2
	pbd
	temporal
	zita-convolver
	zita-resampler
)

if(APPLE)
	target_link_libraries(ardourlib PUBLIC
		appleutility
        "$<LINK_LIBRARY:FRAMEWORK,AudioToolbox>"
    )
endif()


