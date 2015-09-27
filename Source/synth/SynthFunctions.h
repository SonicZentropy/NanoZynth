/*==============================================================================
//  SynthFunctions.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/27
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Generalized Synth Functions
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#ifndef SYNTHFUNCTIONS_H_INCLUDED
#define SYNTHFUNCTIONS_H_INCLUDED

#define CONVEX_LIMIT 0.00398107
#define CONCAVE_LIMIT 0.99601893
#define ARC4RANDOMMAX 4294967295 // (2^32 - 1)

#define EXTRACT_BITS(the_val, bits_start, bits_len) ((the_val >> (bits_start - 1)) & ((1 << bits_len) - 1))

//----------------------------------------------------------------
// --- MIDI Constants
//----------------------------------------------------------------
//
// --- CHANNEL VOICE MESSAGES
const unsigned char NOTE_OFF = 0x80;
const unsigned char NOTE_ON = 0x90;
const unsigned char POLY_PRESSURE = 0xA0;
const unsigned char CONTROL_CHANGE = 0xB0;
const unsigned char PROGRAM_CHANGE = 0xC0;
const unsigned char CHANNEL_PRESSURE = 0xD0;
const unsigned char PITCH_BEND = 0xE0;

// --- CONTINUOUS CONTROLLERS
const unsigned char MOD_WHEEL = 0x01;
const unsigned char VOLUME_CC07 = 0x07;
const unsigned char PAN_CC10 = 0x0A;
const unsigned char EXPRESSION_CC11 = 0x0B;
const unsigned char JOYSTICK_X = 0x10;
const unsigned char JOYSTICK_Y = 0x11;
const unsigned char SUSTAIN_PEDAL = 0x40;
const unsigned char RESET_ALL_CONTROLLERS = 0x79;
const unsigned char ALL_NOTES_OFF = 0x7B;

// --- SYSTEM MESSAGES
const unsigned char SYSTEM_EXCLUSIVE = 0xF0;
const unsigned char MIDI_TIME_CODE = 0xF1;
const unsigned char SONG_POSITION_POINTER = 0xF2;
const unsigned char SONG_SELECT = 0xF3;
const unsigned char TUNE_REQUEST = 0xF6;
const unsigned char END_OF_EXCLUSIVE = 0xF7;
const unsigned char TIMING_CLOCK = 0xF8;
const unsigned char START = 0xFA;
const unsigned char CONTINUE = 0xFB;
const unsigned char SToP = 0xFC;
const unsigned char ACTIVE_SENSING = 0xFE;
const unsigned char SYSTEM_RESET = 0xFF;

// --- FOR Synth Projects
enum midiChannels
{
	MIDI_CH_1 = 0, MIDI_CH_2, MIDI_CH_3, MIDI_CH_4, MIDI_CH_5, MIDI_CH_6, MIDI_CH_7,
	MIDI_CH_8, MIDI_CH_9, MIDI_CH_10, MIDI_CH_11, MIDI_CH_12, MIDI_CH_13,
	MIDI_CH_14, MIDI_CH_15, MIDI_CH_16, MIDI_CH_ALL
};
//----------------------------------------------------------------
// --- END  MIDI Constants
//----------------------------------------------------------------

// --- Modulation Matrix Stuff --- //
enum sources
{
	SOURCE_NONE = 0,

	SOURCE_UI_OSC1_FO,
	SOURCE_UI_OSC2_FO,
	SOURCE_UI_TO_OSC3_FO,
	SOURCE_UI_TO_OSC4_FO,
	SOURCE_UI_TO_ALL_OSC_FO,

	SOURCE_UI_FILTER1_FC,
	SOURCE_UI_FILTER2_FC,
	SOURCE_UI_ALL_FILTER_FC,
	SOURCE_UI_FILTER1_Q,
	SOURCE_UI_FILTER2_Q,
	SOURCE_UI_ALL_FILTER_Q,

	SOURCE_LFO1,
	SOURCE_LFO2,
	SOURCE_LFO1Q,					/* QuadPhase outputs */
	SOURCE_LFO2Q,
	SOURCE_EG1,						/* Exponential outputs */
	SOURCE_EG2,
	SOURCE_EG3,
	SOURCE_EG4,
	SOURCE_BIASED_EG1,				/* Exponential outputs with Sustain subtracted*/
	SOURCE_BIASED_EG2,
	SOURCE_BIASED_EG3,
	SOURCE_BIASED_EG4,
	SOURCE_VELOCITY,
	SOURCE_MIDI_VOLUME_CC07,		/* MIDI Volume */
	SOURCE_MIDI_PAN_CC10,			/* MIDI Pan */
	SOURCE_MIDI_EXPRESSION_CC11,	/* MIDI Expression */
	SOURCE_MODWHEEL,
	SOURCE_PITCHBEND,
	SOURCE_SUSTAIN_PEDAL,
	SOURCE_MIDI_NOTE_NUM,
	SOURCE_MIDI_JS_X,
	SOURCE_MIDI_JS_Y,
	MAX_SOURCES
};

enum destinations
{
	DEST_NONE = 0,

	// --- GUI control layer modulation (-1)
	DEST_UI_OSC1_FO,
	DEST_UI_OSC2_FO,
	DEST_UI_TO_OSC3_FO,
	DEST_UI_TO_OSC4_FO,
	DEST_UI_TO_ALL_OSC_FO,

	DEST_UI_FILTER1_FC,
	DEST_UI_FILTER2_FC,
	DEST_UI_ALL_FILTER_FC,
	DEST_UI_FILTER1_Q,
	DEST_UI_FILTER2_Q,
	DEST_UI_ALL_FILTER_Q,

	// --- LAYER 0 DESTINATIONS 
	//     add more L0 destinations in this chunk
	//     see CModulationMatrix::checkDestinationLayer())
	DEST_LFO1_FO, // <- keep this first
	DEST_LFO2_FO,
	DEST_ALL_LFO_FO,
	DEST_LFO1_OUTPUT_AMP,
	DEST_LFO2_OUTPUT_AMP,
	DEST_ALL_LFO_OUTPUT_AMP,
	DEST_EG1_ATTACK_SCALING,
	DEST_EG2_ATTACK_SCALING,
	DEST_EG3_ATTACK_SCALING,
	DEST_EG4_ATTACK_SCALING,
	DEST_ALL_EG_ATTACK_SCALING,
	DEST_EG1_DECAY_SCALING,
	DEST_EG2_DECAY_SCALING,
	DEST_EG3_DECAY_SCALING,
	DEST_EG4_DECAY_SCALING,
	DEST_ALL_EG_DECAY_SCALING,
	DEST_EG1_SUSTAIN_OVERRIDE,
	DEST_EG2_SUSTAIN_OVERRIDE,
	DEST_EG3_SUSTAIN_OVERRIDE,
	DEST_EG4_SUSTAIN_OVERRIDE,
	DEST_ALL_EG_SUSTAIN_OVERRIDE, // <- keep this last
								  // --- END OF LAYER 0 DESTINATIONS

								  // --- LAYER 1 DESTINATIONS 
	DEST_HARD_SYNC_RATIO,
	DEST_OSC1_FO,
	DEST_OSC2_FO,
	DEST_OSC3_FO,
	DEST_OSC4_FO,
	DEST_ALL_OSC_FO,
	DEST_OSC1_PULSEWIDTH,
	DEST_OSC2_PULSEWIDTH,
	DEST_OSC3_PULSEWIDTH,
	DEST_OSC4_PULSEWIDTH,
	DEST_ALL_OSC_PULSEWIDTH,
	DEST_OSC1_FO_RATIO,
	DEST_OSC2_FO_RATIO,
	DEST_OSC3_FO_RATIO,
	DEST_OSC4_FO_RATIO,
	DEST_ALL_OSC_FO_RATIO,
	DEST_OSC1_OUTPUT_AMP,
	DEST_OSC2_OUTPUT_AMP,
	DEST_OSC3_OUTPUT_AMP,
	DEST_OSC4_OUTPUT_AMP,
	DEST_ALL_OSC_OUTPUT_AMP,
	DEST_FILTER1_FC,
	DEST_FILTER2_FC,
	DEST_ALL_FILTER_FC,
	DEST_FILTER1_KEYTRACK,
	DEST_FILTER2_KEYTRACK,
	DEST_ALL_FILTER_KEYTRACK, // the control value, overriding the GUI
	DEST_FILTER1_Q,
	DEST_FILTER2_Q,
	DEST_ALL_FILTER_Q,
	DEST_VS_AC_AXIS,
	DEST_VS_BD_AXIS,
	DEST_DCA_VELOCITY,
	DEST_DCA_EG,
	DEST_DCA_AMP,
	DEST_DCA_PAN,
	// --- END OF LAYER 1 DESTINATIONS

	MAX_DESTINATIONS
};

enum transform
{
	TRANSFORM_NONE = 0,
	TRANSFORM_UNIPOLAR_TO_BIPOLAR,
	TRANSFORM_BIPOLAR_TO_UNIPOLAR,
	TRANSFORM_MIDI_NORMALIZE,
	TRANSFORM_INVERT_MIDI_NORMALIZE,
	TRANSFORM_MIDI_TO_BIPOLAR,
	TRANSFORM_MIDI_TO_PAN,
	TRANSFORM_MIDI_SWITCH,
	TRANSFORM_MIDI_TO_ATTENUATION,
	TRANSFORM_NOTE_NUMBER_TO_FREQUENCY,
	MAX_TRANSFORMS /* not needed? */
};

#endif // SYNTHFUNCTIONS_H_INCLUDED
