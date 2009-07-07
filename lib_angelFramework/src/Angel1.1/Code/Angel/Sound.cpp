/*
	File: sound.cpp
	Sound class
*/

#include <stdafx.h>
#include "Sound.h"
#include <GL\freeglut_ext.h>

#include <assert.h>

// Defines
#define VOLUME_TRANSLATION_TABLE_SIZE 256

// Structures
struct RIFFChunkHeader {
	unsigned char id[4];
	unsigned int dwSizeOfNextChunk;
};

// Variables
static int Volume_translation_table[256];

// Private Functions

// THIS FUNCTION IS NOT PROTECTED
// SEND THE CORRECT SIZED ARRAYS
void ConvertStrToWide(const char *string,WCHAR *wide_string)
{
	const char *string_ptr=string;
	WCHAR *wide_string_ptr=wide_string;
	while(*string_ptr!=NULL) {
		*wide_string_ptr=*string_ptr;
		wide_string_ptr++;
		string_ptr++;
	}

	*wide_string_ptr=NULL;
}

SoundObj* SoundObj::s_SoundObj = NULL;

SoundObj &SoundObj::GetInstance()
{
	if( s_SoundObj == NULL )
	{
		s_SoundObj = new SoundObj();
		s_SoundObj->Initialize();
	}
	return *s_SoundObj;
}

void SoundObj::Destroy()
{
	if (s_SoundObj != NULL)
	{
		theSound.Terminate();

		delete s_SoundObj;
		s_SoundObj = NULL;
	}
}

// Create a sound from a given WAV file (full path and extension needed)
// 
// name - the full name of the file we are loading
// returns - the newly allocated sound sample
Sound *SoundObj::CreateSoundFromFile(const char *filename)
{
	// Create sound
	Sound *new_sound=new Sound;
	memset(new_sound,0,sizeof(Sound));

	// Set our defaults
	new_sound->loop=false;
	new_sound->volume=1.0f;
	new_sound->pan=0.0f;
	new_sound->play_rate=44100;

	new_sound->current_state=SS_STOPPED;

	// Add to sound list
	new_sound->next=sound_list_header;
	new_sound->prev=NULL;
	if(sound_list_header) {
		sound_list_header->prev=new_sound;
	}
	sound_list_header=new_sound;

	// Try to load WAV
	if(LoadWaveFile(new_sound,filename)!=0) {
		printf("Could not load sound: %s\n",filename);
		delete new_sound;
		sound_list_header = NULL;
		return NULL;
	}

	// Copy unextended sample name
	int name_len=strlen(filename)+1;
	new_sound->name=new char[name_len];
	strcpy_s(new_sound->name,name_len,filename);

	return new_sound;
}

// Create a sound by duplicating an existing sound
// 
// sound_to_duplicate - the sound we're going to duplicate
// returns - the newly allocated sound
Sound *SoundObj::CreateSoundFromDuplication(Sound *sound_to_duplicate)
{
	// Create sound
	Sound *new_sound=new Sound;
	memset(new_sound,0,sizeof(Sound));

	// Set our defaults
	new_sound->loop=sound_to_duplicate->loop;
	new_sound->volume=sound_to_duplicate->volume;
	new_sound->pan=sound_to_duplicate->pan;
	new_sound->play_rate=sound_to_duplicate->play_rate;

	new_sound->buffer_size=sound_to_duplicate->buffer_size;
	new_sound->bits_per_sample=sound_to_duplicate->bits_per_sample;
	new_sound->recorded_rate=sound_to_duplicate->recorded_rate;
	new_sound->stereo=sound_to_duplicate->stereo;

	new_sound->current_state=SS_STOPPED;

	// Add to sound list
	new_sound->next=sound_list_header;
	new_sound->prev=NULL;
	if(sound_list_header) {
		sound_list_header->prev=new_sound;
	}
	sound_list_header=new_sound;

	HRESULT hr=direct_sound->DuplicateSoundBuffer(sound_to_duplicate->direct_sound_buffer,(LPDIRECTSOUNDBUFFER *)&(new_sound->direct_sound_buffer));
	if(hr!=DS_OK) {
		delete new_sound;
		return NULL;
	}

	// Copy unextended sample name
	int name_len=strlen(sound_to_duplicate->name)+1;
	new_sound->name=new char[name_len];
	strcpy_s(new_sound->name,name_len,sound_to_duplicate->name);

	return new_sound;
}

// Protected Functions

// Load all important data from a given WAV file into a sample
// 
// cur_sound - the sound we are filling out
// filename - the fully extended filename we are reading from
// returns - 0 if successful; otherwise an error code
int SoundObj::LoadWaveFile(Sound *cur_sound,const char *filename)
{
	// Open file
	WCHAR unicode_filename[1024];
	ConvertStrToWide(filename,unicode_filename);

	HANDLE file=CreateFile(unicode_filename,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,NULL);
	if(file==INVALID_HANDLE_VALUE) {
		DWORD error=GetLastError();
		return SER_COULD_NOT_OPEN_FILE;
	}

	// Make sure we're opening a RIFF file
	RIFFChunkHeader riff_chunk_header;
	DWORD num_bytes_read=0;
	ReadFile(file,&riff_chunk_header,sizeof(RIFFChunkHeader),&num_bytes_read,NULL);
	if(num_bytes_read!=sizeof(RIFFChunkHeader)) {
		CloseHandle(file);
		return SER_COULD_NOT_READ_FROM_FILE;
	}

	if((riff_chunk_header.id[0]!='R')||(riff_chunk_header.id[1]!='I')||
		(riff_chunk_header.id[2]!='F')||(riff_chunk_header.id[3]!='F')) {
		CloseHandle(file);
		return SER_NOT_RIFF_FILE;
	}

	// Make sure we're opening a WAVE file
	unsigned char riff_type[4];
	ReadFile(file,&riff_type,sizeof(riff_type),&num_bytes_read,NULL);
	if(num_bytes_read!=sizeof(riff_type)) {
		CloseHandle(file);
		return SER_COULD_NOT_READ_FROM_FILE;
	}

	if((riff_type[0]!='W')||(riff_type[1]!='A')||
		(riff_type[2]!='V')||(riff_type[3]!='E')) {
		CloseHandle(file);
		return SER_NOT_WAVE_FILE;
	}

	// Make sure we have a format chunk
	ReadFile(file,&riff_chunk_header,sizeof(RIFFChunkHeader),&num_bytes_read,NULL);
	if(num_bytes_read!=sizeof(RIFFChunkHeader)) {
		CloseHandle(file);
		return SER_COULD_NOT_READ_FROM_FILE;
	}

	if((riff_chunk_header.id[0]!='f')||(riff_chunk_header.id[1]!='m')||
		(riff_chunk_header.id[2]!='t')||(riff_chunk_header.id[3]!=' ')) {
		CloseHandle(file);
		return SER_NO_FORMAT_CHUNK;
	}

	// Make sure the format chunk is large enough
	if(riff_chunk_header.dwSizeOfNextChunk<16) {
		CloseHandle(file);
		return SER_CORRUPT_FORMAT_CHUNK;
	}

	// Load our WAVE format chunk
	unsigned char *format_chunk=new unsigned char [riff_chunk_header.dwSizeOfNextChunk];
	ReadFile(file,format_chunk,riff_chunk_header.dwSizeOfNextChunk,&num_bytes_read,NULL);
	if(num_bytes_read!=int(riff_chunk_header.dwSizeOfNextChunk)) {
		CloseHandle(file);
		return SER_COULD_NOT_READ_FROM_FILE;
	}

	// WAVE format chunk minimum requirements
	// unsigned short wFormatTag;			// 1=PCM
	// unsigned short wChannels;			// 1 for mono, 2 for stereo
	// unsigned int dwSamplesPerSec;		// number of samples per second per channel
	// unsigned int dwAvgBytesPerSec;		// wChannels*nBitsPerSecond*(wBitsPerSample/8)
	// unsigned short wBlockAlign;			// wChannels*wBitsPerSample/8
	// unsigned short wBitsPerSample;		// 8 or 16 (PCM only)
	// <any more after this is ignored>

	// Parse the format chunk
	unsigned char *temp_ptr=format_chunk;

	// Get our data format (PCM or ADPCM)
	unsigned short data_format=*((unsigned short *)temp_ptr);
	temp_ptr+=2;
	if(data_format!=1) {
		CloseHandle(file);
		delete []format_chunk;
		return SER_UNKNOWN_DATA_FORMAT;
	}

	// Get the number of channels
	unsigned short num_channels=*((unsigned short *)temp_ptr);
	temp_ptr+=2;
	if(num_channels>2) {
		CloseHandle(file);
		delete []format_chunk;
		return SER_DATA_HAS_TOO_MANY_CHANNELS;
	}

	cur_sound->stereo=false;
	if(num_channels==2) {
		cur_sound->stereo=true;
	}

	// Get play rate
	cur_sound->recorded_rate=*((unsigned int *)temp_ptr);
	temp_ptr+=4;
	if(cur_sound->recorded_rate<MIN_RATE) {
		CloseHandle(file);
		delete []format_chunk;
		return SER_DATA_SAMPLE_RATE_TOO_LOW;
	}
	if(cur_sound->recorded_rate>MAX_RATE) {
		CloseHandle(file);
		delete []format_chunk;
		return SER_DATA_SAMPLE_RATE_TOO_HIGH;
	}

	// Dump average bytes per second
//	unsigned int average_bytes_per_second=*((unsigned int *)temp_ptr);
	temp_ptr+=4;

	// Dump block alignment
//	unsigned short block_alignment=*((unsigned short *)temp_ptr);
	temp_ptr+=2;

	// Get bits per second
	unsigned short bits_per_sample=*((unsigned short *)temp_ptr);
	temp_ptr+=2;
	if((bits_per_sample!=16)&&(bits_per_sample!=8)&&(bits_per_sample!=4)) {
		CloseHandle(file);
		delete []format_chunk;
		return SER_BAD_BITS_PER_SECOND;
	}

	cur_sound->bits_per_sample=bits_per_sample;

	delete []format_chunk;

	// Get the data chunk
	ReadFile(file,&riff_chunk_header,sizeof(RIFFChunkHeader),&num_bytes_read,NULL);
	if(num_bytes_read!=sizeof(RIFFChunkHeader)) {
		CloseHandle(file);
		return SER_COULD_NOT_READ_FROM_FILE;
	}

	if((riff_chunk_header.id[0]=='f')&&(riff_chunk_header.id[1]=='a')&&
		(riff_chunk_header.id[2]=='c')&&(riff_chunk_header.id[3]=='t')) {
		// Skip past the fact chunk
		cur_sound->buffer_size=riff_chunk_header.dwSizeOfNextChunk;
		unsigned char *temp_data=new unsigned char[cur_sound->buffer_size];
		ReadFile(file,temp_data,cur_sound->buffer_size,&num_bytes_read,NULL);
		if(num_bytes_read!=int(cur_sound->buffer_size)) {
			delete []temp_data;
			CloseHandle(file);
			return SER_COULD_NOT_READ_FROM_FILE;
		}

		// Trash the useless data and read the next chunk header
		delete []temp_data;
		ReadFile(file,&riff_chunk_header,sizeof(RIFFChunkHeader),&num_bytes_read,NULL);
		if(num_bytes_read!=sizeof(RIFFChunkHeader)) {
			CloseHandle(file);
			return SER_COULD_NOT_READ_FROM_FILE;
		}
	}

	if((riff_chunk_header.id[0]!='d')||(riff_chunk_header.id[1]!='a')||
		(riff_chunk_header.id[2]!='t')||(riff_chunk_header.id[3]!='a')) {
		CloseHandle(file);
		return SER_NO_DATA_CHUNK;
	}

	cur_sound->buffer_size=riff_chunk_header.dwSizeOfNextChunk;

	// Load data for sample

	// Set up WAV format structure
	WAVEFORMATEX wfx;
	memset(&wfx,0,sizeof(WAVEFORMATEX));
	wfx.wFormatTag=WAVE_FORMAT_PCM;
	wfx.nChannels=num_channels;
	wfx.nSamplesPerSec=cur_sound->recorded_rate;
	wfx.wBitsPerSample=cur_sound->bits_per_sample;
	wfx.nBlockAlign=(num_channels*wfx.wBitsPerSample)/8;
	wfx.nAvgBytesPerSec=wfx.nSamplesPerSec*wfx.nBlockAlign;

	// Set up DSBUFFERDESC structure
	DSBUFFERDESC dsbdesc;
	memset(&dsbdesc,0,sizeof(DSBUFFERDESC));
	dsbdesc.dwSize=sizeof(DSBUFFERDESC);
	dsbdesc.dwFlags=DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLPAN|DSBCAPS_CTRLFREQUENCY|DSBCAPS_GLOBALFOCUS|DSBCAPS_LOCSOFTWARE;
	dsbdesc.dwBufferBytes=cur_sound->buffer_size;
	dsbdesc.lpwfxFormat=&wfx;

	// Create buffer
	LPDIRECTSOUNDBUFFER sound_buffer_ptr=NULL;
	HRESULT hr=direct_sound->CreateSoundBuffer(&dsbdesc,&sound_buffer_ptr,NULL);
	if(SUCCEEDED(hr)) { 
		hr=sound_buffer_ptr->QueryInterface(IID_IDirectSoundBuffer8,(LPVOID *)&(cur_sound->direct_sound_buffer));
		sound_buffer_ptr->Release();
	}

	LPVOID start_ptr;
	DWORD size;
	if(cur_sound->direct_sound_buffer->Lock(0,0,&start_ptr,&size,NULL,NULL,DSBLOCK_ENTIREBUFFER)==DS_OK) {
		ReadFile(file,start_ptr,size,&num_bytes_read,NULL);
		if(num_bytes_read!=int(cur_sound->buffer_size)) {
			cur_sound->direct_sound_buffer->Unlock(start_ptr,size,NULL,0);
			CloseHandle(file);
			return SER_COULD_NOT_READ_FROM_FILE;
		}
	}

	cur_sound->direct_sound_buffer->Unlock(start_ptr,size,NULL,0);
	CloseHandle(file);

	return SER_NONE;
}

SoundObj::SoundObj()
{
	// Currently illegal to call directly
	// Must only be called through CreateSoundObj
	sound_list_header=NULL;

	dsound_inited=false;
	direct_sound=NULL;
}

// Public Functions
SoundObj::~SoundObj()
{
}

int SoundObj::Initialize()
{
	if(dsound_inited) {
		return SER_NONE;
	}

	// Create DirectSound
	DirectSoundCreate8(NULL,&direct_sound,NULL);
	direct_sound->SetCooperativeLevel(GiveMeTheKnife(),DSSCL_PRIORITY);

	// Calculate our volume translation table.
	// DirectSound uses a -10000 -> 0 range where units
	// indicate hundreths of decibles.  That's some fucked
	// up shit mang.
	Volume_translation_table[0]=-10000;
	for(int i=1; i<VOLUME_TRANSLATION_TABLE_SIZE; i++) {
		float volume=i/float(VOLUME_TRANSLATION_TABLE_SIZE-1);
		Volume_translation_table[i]=int((log(volume)/log(2.0f))*1000.0f);
	}

	dsound_inited=true;

	return SER_NONE;
}

int SoundObj::Terminate()
{
	// Delete all sounds remaining in the system
	while(sound_list_header) {
		Sound *next_sound=sound_list_header->next;
		UnloadSound(sound_list_header);

		sound_list_header=next_sound;
	}

	if(direct_sound) {
		direct_sound->Release();
	}

	dsound_inited=false;

	return SER_NONE;
}

int SoundObj::StopAllSounds()
{
	Sound *cur_sound=sound_list_header;
	while(cur_sound) {
		StopSound(cur_sound);
		cur_sound=cur_sound->next;
	}

	return SER_NONE;
}

Sound *SoundObj::LoadSound(const char *filename)
{
	if(filename==NULL) {
		return NULL;
	}

	// Find if this sound has already been loaded
	Sound *cur_sound=sound_list_header;
	while(cur_sound) {
		if(strcmp(cur_sound->name,filename)==0) {
			break;
		}
		cur_sound=cur_sound->next;
	}

	Sound *new_sound=NULL;
	if(cur_sound==NULL) {
		// No sound found, load one
		new_sound=CreateSoundFromFile(filename);
		if(new_sound==NULL) {
			return NULL;
		}
	} else {
		// We need to duplicate the sound
		new_sound=CreateSoundFromDuplication(cur_sound);
		if(new_sound==NULL) {
			return NULL;
		}
	}

	return new_sound;
}

int SoundObj::UnloadSound(Sound *cur_sound)
{
	if(cur_sound==NULL) {
		return SER_NULL_SOUND;
	}

	StopSound(cur_sound);

	// Now it is safe to do this?
	if(cur_sound->direct_sound_buffer) {
		cur_sound->direct_sound_buffer->Release();
	}

	if(cur_sound->name) {
		delete [](cur_sound->name);
	}

	// Unlink sound from list
	if(cur_sound->next) {
		cur_sound->next->prev=cur_sound->prev;
	}
	if(cur_sound->prev) {
		cur_sound->prev->next=cur_sound->next;
	} else {
		// Deleted the first element
		sound_list_header=cur_sound->next;
	}

	delete cur_sound;

	return SER_NONE;
}

int SoundObj::SoundLengthInMilliseconds(Sound *cur_sound)
{
	if(cur_sound==NULL) {
		return 0;
	}

	float bytes_per_sample=0;
	switch(cur_sound->bits_per_sample) {
	case 4:
		bytes_per_sample=0.5f;
		break;
	case 8:
		bytes_per_sample=1.0f;
		break;
	case 16:
		bytes_per_sample=2.0f;
		break;
	default:
//		SPBreak("Unsupported bits per sample");
		break;
	}

	if(cur_sound->stereo) {
		bytes_per_sample*=2.0f;
	}

	return int(((cur_sound->buffer_size/bytes_per_sample)/cur_sound->recorded_rate)*1000.0f);
}

int SoundObj::PlaySound(Sound *cur_sound,float volume,bool loop,int flags)
{
	if(cur_sound==NULL) {
		return SER_NULL_SOUND;
	}

	if(flags&PSF_STOP_SOUND) {
		StopSound(cur_sound);
	}

	//rb - Without this, a sound will never be available again.
	DWORD dwStatus;
	cur_sound->direct_sound_buffer->GetStatus(&dwStatus);

	if (dwStatus == 0)
		cur_sound->current_state = SS_STOPPED;

	if(cur_sound->current_state==SS_PLAYING) {
		return SER_SOUND_ALREADY_PLAYING;
	}

	// -------------------------------------------------
	// This is where we set all the playback attributes
	//--------------------------------------------------

	// Default 2d layout
	cur_sound->pan=0.0f;
	cur_sound->loop=loop;

	SetVolume(cur_sound,volume);
	SetPan(cur_sound,cur_sound->pan);
	SetRate(cur_sound,cur_sound->recorded_rate);

	// -------------------------------------------------
	// This is where we actually start the sound playing
	//--------------------------------------------------
	HRESULT hr=cur_sound->direct_sound_buffer->SetCurrentPosition(0);

	cur_sound->current_state=SS_PLAYING;
	if(cur_sound->loop) {
		hr=cur_sound->direct_sound_buffer->Play(0,0,DSBPLAY_LOOPING);
	} else {
		hr=cur_sound->direct_sound_buffer->Play(0,0,0);
	}

	return SER_NONE;
}

int SoundObj::StopSound(Sound *cur_sound)
{
	if(cur_sound==NULL) {
		return SER_NULL_SOUND;
	}

	if(cur_sound->current_state==SS_STOPPED) {
		return SER_NONE;
	}

	cur_sound->current_state=SS_STOPPED;
	cur_sound->direct_sound_buffer->Stop();
	cur_sound->direct_sound_buffer->SetCurrentPosition(0);

	return SER_NONE;
}

int SoundObj::PauseSound(Sound *cur_sound)
{
	if(cur_sound==NULL) {
		return SER_NULL_SOUND;
	}

	if(cur_sound->current_state==SS_STOPPED) {
		return SER_SOUND_NOT_PLAYING;
	}

	// Simply stop the sound but keep the cursor in place
	cur_sound->direct_sound_buffer->Stop();

	return SER_NONE;
}

int SoundObj::UnpauseSound(Sound *cur_sound)
{
	if(cur_sound==NULL) {
		return SER_NULL_SOUND;
	}

	if(cur_sound->current_state==SS_STOPPED) {
		return SER_SOUND_NOT_PAUSED;
	}

	// Start the sound up again
	if(cur_sound->loop) {
		cur_sound->direct_sound_buffer->Play(0,0,DSBPLAY_LOOPING);
	} else {
		cur_sound->direct_sound_buffer->Play(0,0,0);
	}

	return SER_NONE;
}

int SoundObj::IsSoundPlaying(Sound *cur_sound)
{
	if(cur_sound==NULL) {
		return 0;
	}

	if(cur_sound->current_state==SS_PLAYING) {
		return 1;
	}

	return 0;
}

// Set the requested volume for a given sound
// Range: 0.0f to 1.0f with 0.0f being silent
// 
// cur_sound - the sound we are setting the requested volume of
// new_volume - the requested volume
// returns - 0 if successful; otherwise an error code
int SoundObj::SetVolume(Sound *cur_sound,float new_volume)
{
	if(cur_sound==NULL) {
		return SER_NULL_SOUND;
	}

	// Clip sound volume
	if(new_volume<MIN_VOLUME) {
		new_volume=MIN_VOLUME;
	}
	if(new_volume>MAX_VOLUME) {
		new_volume=MAX_VOLUME;
	}
	cur_sound->volume=new_volume;

	int index=int(new_volume*(VOLUME_TRANSLATION_TABLE_SIZE-1));
	assert(index<VOLUME_TRANSLATION_TABLE_SIZE);
	assert(index>=0);

	HRESULT hr=cur_sound->direct_sound_buffer->SetVolume(Volume_translation_table[index]);
	if(hr!=DS_OK) {
		return SER_SET_VOLUME_FAILED;
	}

	return SER_NONE;
}

// Get the requested volume for a given sound
// Range: 0.0f to 1.0f with 0.0f being silent
// 
// cur_sound - the sound we are getting the requested volume of
// returns - the requested volume for this sound
float SoundObj::Volume(Sound *cur_sound)
{
	if(cur_sound==NULL) {
		return 0.0f;
	}

	return cur_sound->volume;
}

// Set the channel strengths for a given sound
// All values are in the range 0.0f to 1.0f with 0.0f being silent
// 
// cur_sound - the current sound we are affecting
// returns - 0 if successful; otherwise an error code
int SoundObj::SetPan(Sound *cur_sound,float new_pan)
{
	if(cur_sound==NULL) {
		return SER_NULL_SOUND;
	}

	// Clip pan to correct range
	if(new_pan<DSBPAN_LEFT) {
		new_pan=DSBPAN_LEFT;
	}
	if(new_pan>DSBPAN_RIGHT) {
		new_pan=DSBPAN_RIGHT;
	}

	cur_sound->pan=new_pan;

	LPDIRECTSOUNDBUFFER8 cur_buffer=cur_sound->direct_sound_buffer;
	if(cur_buffer) {
		int ds_pan;
		if(new_pan>=0.0f) {
			int pan_index=int((1.0f-new_pan)*(VOLUME_TRANSLATION_TABLE_SIZE-1));
			ds_pan=-Volume_translation_table[pan_index];
		} else {
			int pan_index=int((new_pan+1.0f)*(VOLUME_TRANSLATION_TABLE_SIZE-1));
			ds_pan=Volume_translation_table[pan_index];
		}

		HRESULT hr=cur_buffer->SetPan(ds_pan);
		if(hr!=DS_OK) {
			return SER_SET_PAN_FAILED;
		}
	}

	return SER_NONE;
}

// Get the channel strengths for a given sound
// All values are in the range 0.0f to 1.0f with 0.0f being silent
// 
// cur_sound - the current sound we are affecting
float SoundObj::Pan(Sound *cur_sound)
{
	if(cur_sound==NULL) {
		return 0.0f;
	}

	return cur_sound->pan;
}

// Set the requested play rate value for a given sound
// 
// cur_sound - the current sound we are affecting
// new_play_rate - the new rate state
// returns - 0 if successful; otherwise an error code
int SoundObj::SetRate(Sound *cur_sound,int new_play_rate)
{
	if(cur_sound==NULL) {
		return SER_NULL_SOUND;
	}

	// Clip sound rate
	if(new_play_rate<MIN_RATE) {
		new_play_rate=MIN_RATE;
	}
	if(new_play_rate>MAX_RATE) {
		new_play_rate=MAX_RATE;
	}
	cur_sound->play_rate=new_play_rate;

	HRESULT hr=cur_sound->direct_sound_buffer->SetFrequency(new_play_rate);
	if(hr!=DS_OK) {
		return SER_SET_FREQUENCY_FAILED;
	}

	return SER_NONE;
}

// Get the requested play rate for a given sound
// 
// cur_sound - the sound we are questioning
// returns - the requested play rate of the given sound
int SoundObj::Rate(Sound *cur_sound)
{
	if(cur_sound==NULL) {
		return 0;
	}

	return cur_sound->play_rate;
}

// This passes back a pointer to the Direct Sound System
// This should be used with EXTREME caution
// 
// returns - the current sound system
void *SoundObj::SoundSystem()
{
	return direct_sound;
}
