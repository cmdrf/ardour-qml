add_library(fluidsynth STATIC
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
)

target_include_directories(fluidsynth PUBLIC
	${L}/fluidsynth/fluidsynth
)

target_include_directories(fluidsynth PRIVATE
	${L}/fluidsynth/
)

target_link_libraries(fluidsynth PUBLIC
	PkgConfig::GLIB2
)
