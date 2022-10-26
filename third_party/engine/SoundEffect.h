/******************************************************************************/
/*!
\file   SoundEffect.h
\author Juyong Jeong
\par    email: jeykop14\@digipen.edu
\par    GAM150 demo
\date   2018/03/14

Using SDL2 for playing sound effect

*****YOU CANNOT MODIFY THIS FILE!!*****
*/
/******************************************************************************/
#pragma once
#include "SDL2/sdl_mixer.h"	// Sound

class SoundEffect {

private:
	
	int			 m_volume = 64;				// Volume range : [0 - 128]
	Mix_Chunk   *m_soundEffect = nullptr;   // Pointer to music
	
public:

    SoundEffect() {};
    ~SoundEffect() {};

	// Load the sound file by passing a directory
	// *Only takes .wav files
	void LoadSound(const char* filePath);
	// Deallocate the sound
	void Free(); 
	// Play sound effect
	// It will repeat {loop} times
	void Play(int loop = 0);
	// Set sound effect volume
	// 0 - 128
	void SetVolume(int volume);
	// Get sound effect volume;
	int  GetVolume();

private:

	SoundEffect(const SoundEffect&) = delete;
	SoundEffect(SoundEffect&&) = delete;
	SoundEffect& operator=(const SoundEffect&) = delete;
	SoundEffect& operator=(SoundEffect&&) = delete;

};
