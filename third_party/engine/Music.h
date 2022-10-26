/******************************************************************************/
/*!
\file   Music.h
\author Juyong Jeong
\par    email: jeykop14\@digipen.edu
\par    GAM150 demo
\date   2018/03/14

Using SDL2 for playing music

*****YOU CANNOT MODIFY THIS FILE!!*****
*/
/******************************************************************************/
#pragma once
#include "SDL2/sdl_mixer.h"	// Sound

class Music {

private:

	Mix_Music   *m_music = nullptr; // Pointer to music
	int			m_volume = 64;      // Volume range : [0 - 128]

public:

	Music() {};
	~Music() {};
	
	// Decallocate the music
	void Free();
	// Load the music file by passing a path  
	void LoadMusic(const char* filePath);
	// Check if the music is playing
	bool IsPlaying();
	// Check if the music is paused
	bool IsPaused();
	// Play music {loop} time
	// -1 means "forever"
	void Play(int loop = -1);
	// Pause the music
	void Pause();
	// Resume the music
	void Resume();
	// Halt the music 
	void Stop();
	// Rewind the music from the start
	void Rewind();

	// Set music volume
	// 0 - 128
	void SetVolume(int volume);
	// Get music volume;
	int  GetVolume();

private:

	Music(const Music&) = delete;
	Music(Music&&) = delete;
	Music& operator=(const Music&) = delete;
	Music& operator=(Music&&) = delete;
};