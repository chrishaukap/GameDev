/*
	File:		sound.h

	Base sound class
	Units on all length values must be kept constant
	across all fuctnion calls
*/

#ifndef _ANGEL_SOUND_H
#define _ANGEL_SOUND_H

#include <Mmreg.h>
#include <dsound.h>
#include <windows.h>

// Defines
#define theSound SoundObj::GetInstance()

#define MIN_VOLUME 0.0f
#define MAX_VOLUME 1.0f
#define NORMAL_VOLUME 1.0f

#define MIN_RATE 188
#define MAX_RATE 191983

// These defines are sent when playing sounds
#define PSF_STOP_SOUND				(1<<0)

enum SoundErrorReturns {
	SER_NONE,

	// External API errors
	SER_NULL_SOUND,											// Sent in a NULL sound pointer
	SER_SOUND_ALREADY_PLAYING,								// We've been asked to play a sound that is already playing
	SER_SOUND_NOT_PLAYING,									// We've been asked to pause a sound that is not playing
	SER_SOUND_NOT_PAUSED,									// We've been asked to unpause a sound that is not paused

	// File errors
	SER_COULD_NOT_OPEN_FILE,								// File could not be opened
	SER_COULD_NOT_READ_FROM_FILE,							// File could not be read
	SER_NOT_RIFF_FILE,										// File not RIFF format
	SER_NOT_WAVE_FILE,										// File not WAVE format
	SER_NO_FORMAT_CHUNK,									// RIFF format chunk missing (corrupt file)
	SER_CORRUPT_FORMAT_CHUNK,								// RIFF format chunk is too small (corrupt file)
	SER_UNKNOWN_DATA_FORMAT,								// Data not in PCM or ADPCM format
	SER_DATA_HAS_TOO_MANY_CHANNELS,							// Data has more than 2 channels
	SER_DATA_SAMPLE_RATE_TOO_LOW,							// Data has a sample rate too low for playback
	SER_DATA_SAMPLE_RATE_TOO_HIGH,							// Data has a sample rate too high for playback
	SER_BAD_BITS_PER_SECOND,								// Data is not in 16 or 8 bits per second
	SER_NO_DATA_CHUNK,										// File has no data chunk (corrupt file)

	// Internal errors (probably a bug in our code)
	SER_SET_VOLUME_FAILED,									// Set volume failed... check HRESULT
	SER_SET_PAN_FAILED,										// Set pan failed... check HRESULT
	SER_SET_FREQUENCY_FAILED,								// Set frequency failed... check HRESULT

	SER_NUM_SOUND_ERROR_RETURNS,

	SER_FORCE_32BITS=0x7FFFFFFF
};

enum SoundState {
	SS_NONE,
	SS_STOPPED,
	SS_PLAYING,
	SS_NUM_SOUND_STATES,

	SS_FORCE_32BITS=0x7FFFFFFF
};

// Structures

// An instance of a sample
struct Sound {
	Sound *next;
	Sound *prev;

	char *name;

	float volume;							// 0.0f to 1.0f
	float pan;								// -1.0f (left) to 1.0f (right)
	unsigned int play_rate;
	bool loop;

	unsigned int buffer_size;
	unsigned int bits_per_sample;
	unsigned int recorded_rate;
	unsigned int stereo;

	LPDIRECTSOUNDBUFFER8 direct_sound_buffer;

	SoundState current_state;
};

// Sound Class
class CriticalSection;
class SoundObj {
// Class fields
public:
	static SoundObj* s_SoundObj;
		// The currently active sound system

// Class methods
public:
	static SoundObj &GetInstance();
		// Create a new sound object.  This must be done before any calls to the 
		// sound system can be made.  The object returned depends on the sound 
		// subsystem passed in and it is opaque to the caller.

// Instance fields
private:
	int dsound_inited;

	// Direct Sound
	LPDIRECTSOUND8 direct_sound;

public:
	Sound *sound_list_header;

// Instance methods
private:
	Sound *CreateSoundFromFile(const char *filename);
		// Create a sound from a given WAV file (full path and extension needed)

	Sound *CreateSoundFromDuplication(Sound *sound_to_duplicate);
		// Create a sound by duplicating an existing sound

protected:
	int LoadWaveFile(Sound *cur_sound,const char *filename);
		// Load all important data from a given WAV file into a sound

	SoundObj();
		// It's illegal to create a SoundObj instance directly; 
		// CreateSoundObj must be used.

	SoundObj(const SoundObj &obj);
		// Copy constructor.  Currently illegal.

public:
	int Initialize();
		// After the sound object has been retrieved through CreateSoundObj, 
		// Initialize must be called to perform any sound subsystem initialization.

	static void Destroy();
	int Terminate();
		// Perform any deinitialization upon shutting down the sound system

	int StopAllSounds();
		// Stops all playing or paused sounds

	Sound *LoadSound(const char *filename);
		// Loads and creates sound objects (full path and extension needed)

	int UnloadSound(Sound *sound_to_delete);
		// Unloads and deletes sound objects

	int SoundLengthInMilliseconds(Sound *cur_sound);

	int PlaySound(Sound *cur_sound,float volume,bool loop,int flags);
		// Plays a given sound in 2d space at the given volume, looping if asked
		// NOTE: If sound is already playing then nothing happens unless PSF_STOP_SOUND flag is specified

	int StopSound(Sound *cur_sound);
		// Stops the given sound from playing

	int PauseSound(Sound *snd);
		// Pause a sound
		// If the sound is not playing, then nothing occurs

	int UnpauseSound(Sound *snd);
		// Unpause a sound

	int IsSoundPlaying(Sound *cur_sound);
		// Return true if the given sound is playing; otherwise, false

	int SetVolume(Sound *cur_sound,float new_volume);
		// Set the requested volume for a given sound
		// Range: 0.0f to 1.0f with 0.0f being silent

	float Volume(Sound *cur_sound);
		// Get the requested volume for a given sound
		// Range: 0.0f to 1.0f with 0.0f being silent

	int SetPan(Sound *cur_sound,float new_pan);
		// Set the pan for a given sound
		// Range: -1.0f (left) to 1.0f (right)

	float Pan(Sound *cur_sound);
		// Get the pan for a given sound
		// Range: -1.0f (left) to 1.0f (right)

	int SetRate(Sound *cur_sound,int new_play_rate);
		// Set the requested play rate value for a given sound

	int Rate(Sound *cur_sound);
		// Get the requested play rate for a given sound

	void *SoundSystem();
		// This passes back a pointer to the Direct Sound System
		// This should be used with EXTREME caution

	~SoundObj();
		// As a saftey feature, the deconstructor will call UnloadSound on all
		// still loaded sounds and print out a record of who left sounds
		// allocated in memory
};

#endif // _ANGEL_SOUND_H
