/*
Copyright (c) 2015, Cong Xu
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/
#pragma once

#include <stdbool.h>

#ifdef NOSOUND
	typedef int Mix_Chunk;
	typedef int Mix_Music;
#endif

#ifdef DREAMCAST
#include <kos.h>
#include <string.h>
#endif

#ifndef NOSOUND
#include <SDL_mixer.h>
#endif

#ifdef DREAMCAST
extern sfxhnd_t SoundPlayerRoll;
extern int music;
#else
extern Mix_Chunk* SoundPlayerRoll;
extern Mix_Music *music;
#endif

#ifdef DREAMCAST
void SoundPlay(int sound, const float volume);
#else
void SoundPlay(Mix_Chunk *sound, const float volume);
#endif

void SoundPlayBounce(const float speed);
void SoundPlayRoll(const int player, const float speed);
void SoundStopRoll(const int player);

void MusicSetLoud(const bool fullVolume);

bool SoundLoad(void);
void SoundFree(void);
