#include "sound.h"
#include "common.h"

#include <math.h>

#include "player.h"
#include "utils.h"

#ifdef DREAMCAST
#include <kos.h>
#include <string.h>
#endif


#define BOUNCE_SPEED_MAX_VOLUME 150.0f
#define BOUNCE_SPEED_MIN_VOLUME 10.0f
#define ROLL_SPEED_MAX_VOLUME 20.0f
static int rollChannels[MAX_PLAYERS];

#define MUSIC_VOLUME_LOW 24
#define MUSIC_VOLUME_HIGH 64

#ifndef NOSOUND
#ifdef DREAMCAST
sfxhnd_t SoundPlayerRoll;
int music = 1;
#else
Mix_Music *music;
Mix_Chunk* SoundPlayerRoll = NULL;
#endif
#endif


bool SoundLoad(void)
{
#ifndef NOSOUND

#ifdef DREAMCAST
	SoundPlayerRoll = snd_sfx_load(SOUND_ROLL_FILE);
#else
	SoundPlayerRoll = Mix_LoadWAV_RW(SDL_RWFromFile(SOUND_ROLL_FILE, "rb"), 1);
#endif

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		rollChannels[i] = -1;
	}
#endif
	return true;
}
void SoundFree(void)
{
#ifndef NOSOUND
#ifdef DREAMCAST
	if (SoundPlayerRoll) snd_sfx_unload(SoundPlayerRoll);
#else
	if (SoundPlayerRoll) Mix_FreeChunk(SoundPlayerRoll);
#endif
#endif
}

#ifdef DREAMCAST
void SoundPlay(int sound, const float volume)
#else
void SoundPlay(Mix_Chunk *sound, const float volume)
#endif
{
#ifndef NOSOUND
	#ifdef DREAMCAST
	snd_sfx_play(sound, 255, 128);
	#else
	const int channel = Mix_PlayChannel(-1, sound, 0);
	if (channel >= 0)
	{
		Mix_Volume(channel, (int)round(MIN(1.0f, volume) * MIX_MAX_VOLUME));
	}
	#endif
#endif
}

void SoundPlayBounce(const float speed)
{
	#ifndef NOSOUND
	const float imp = (float)fabs(speed);
	if (imp < BOUNCE_SPEED_MIN_VOLUME) return;
	SoundPlay(SoundPlayerBounce, imp / BOUNCE_SPEED_MAX_VOLUME);
	#endif
}

void SoundPlayRoll(const int player, const float speed)
{
#ifndef NOSOUND
	#ifdef DREAMCAST
		/*if (rollChannels[player] == -1)
		{
			snd_sfx_play_chn(4,SoundPlayerRoll, 255, 128);
		}
		if (rollChannels[player] != -1)
		{
			snd_sfx_stop(4);
		}
		*/
	#else
	if (rollChannels[player] == -1)
	{
		rollChannels[player] = Mix_PlayChannel(-1, SoundPlayerRoll, -1);
	}
	if (rollChannels[player] != -1)
	{
		const float volume = (float)fabs(speed) / ROLL_SPEED_MAX_VOLUME;
		Mix_Volume(
			rollChannels[player],
			(int)round(MIN(1.0f, volume) * MIX_MAX_VOLUME));
	}
	#endif
#endif
}

void SoundStopRoll(const int player)
{
#ifndef NOSOUND
	#ifdef DREAMCAST
		/*if (rollChannels[player] != -1)
		{
			snd_sfx_stop(4);
			rollChannels[player] = -1;
		}*/
	#else
	if (rollChannels[player] != -1)
	{
		Mix_HaltChannel(rollChannels[player]);
		rollChannels[player] = -1;
	}
	#endif
#endif
}


void MusicSetLoud(const bool fullVolume)
{
#ifndef DREAMCAST
#ifndef NOSOUND
	Mix_VolumeMusic(fullVolume ? MUSIC_VOLUME_HIGH : MUSIC_VOLUME_LOW);
#endif
#endif
}
