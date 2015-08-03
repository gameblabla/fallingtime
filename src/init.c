/*
 * Ativayeban, initialisation code file
 * Copyright (C) 2014 Nebuleon Fumika <nebuleon@gcw-zero.com>
 * 2015 Cong Xu <congusbongus@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <stdbool.h>
#include <stddef.h>

#include "common.h"

#include "SDL.h"

#ifdef EMBED
#include "penguin_ball.h"
#include "penguin_black.h"
#include "eggplant.h"
#endif

#include "font.h"

#include "gap.h"
#include "game.h"
#include "main.h"
#include "high_score.h"
#include "init.h"
#include "input.h"
#include "particle.h"
#include "pickup.h"
#include "platform.h"
#include "player.h"
#include "space.h"
#include "sound.h"
#include "title.h"

#ifdef DREAMCAST
#include <kos.h>
#include <string.h>
#include <mp3/sndserver.h>
#include <oggvorbis/sndoggvorbis.h>
uint8 romdisk[];
KOS_INIT_FLAGS(INIT_DEFAULT | INIT_MALLOCSTATS);
KOS_INIT_ROMDISK(romdisk);
#endif

#ifdef EMBED
	SDL_RWops *rw, *rw2, *rw3;
#endif

void Initialize(bool* Continue, bool* Error)
{
	SDL_Surface *tmp;
	
#ifdef NOSOUND
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
#else
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
#endif
	{
		*Continue = false;  *Error = true;
		printf("SDL initialisation failed: %s\n", SDL_GetError());
		SDL_ClearError();
		return;
	}
	else
		printf("SDL initialisation succeeded\n");

#ifdef NSPIRE
	Screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, has_colors ? 16 : 8, SDL_SWSURFACE);
#else
	Screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_SWSURFACE);
#endif

	SDL_ShowCursor(0);

#ifndef NOSOUND
#ifdef DREAMCAST
	snd_stream_init();
	sndoggvorbis_init();
#endif
#endif

	if (Screen == NULL)
	{
		*Continue = false;  *Error = true;
		printf("SDL_SetVideoMode failed: %s\n", SDL_GetError());
		SDL_ClearError();
		return;
	}
	else
		printf("SDL_SetVideoMode succeeded\n");

#ifndef NOSOUND

#ifndef DREAMCAST
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		*Continue = false;  *Error = true;
		printf("Mix_OpenAudio failed: %s\n", SDL_GetError());
		SDL_ClearError();
		return;
	}
	else
		printf("Mix_OpenAudio succeeded\n");
#endif
		
#endif

	InputInit();
	HighScoresInit();

#ifdef TTF
	if (TTF_Init() == -1)
	{
		*Continue = false;  *Error = true;
		printf("TTF_Init failed: %s\n", SDL_GetError());
		SDL_ClearError();
		return;
	}
	else
		printf("TTF_Init succeeded\n");
#endif

	SDL_WM_SetCaption("FallingTime", NULL);

#ifdef EMBED
	rw = SDL_RWFromMem(IMG_PENGUIN_BALL, PENGUIN_BALL_SIZE);
	tmp = SDL_LoadBMP_RW(rw, 0);
	SDL_SetColorKey(tmp, (SDL_SRCCOLORKEY | SDL_ACCELERATION_RLE), SDL_MapRGB(tmp->format, 255, 0, 255));
	PlayerSpritesheets[0] = SDL_DisplayFormat(tmp);
	
	if (tmp) SDL_FreeSurface(tmp);
	if (rw) SDL_FreeRW(rw);
	
	rw = SDL_RWFromMem(IMG_PENGUIN_BLACK, PENGUIN_BLACK_SIZE);
	tmp = SDL_LoadBMP_RW(rw, 0);
	SDL_SetColorKey(tmp, (SDL_SRCCOLORKEY | SDL_ACCELERATION_RLE), SDL_MapRGB(tmp->format, 255, 0, 255));
	PlayerSpritesheets[1] = SDL_DisplayFormat(tmp);
	
	if (tmp) SDL_FreeSurface(tmp);
	if (rw) SDL_FreeRW(rw);
	
	rw = SDL_RWFromMem(IMG_EGGPLANT, EGGPLANT_SIZE);
	tmp = SDL_LoadBMP_RW(rw, 0);
	SDL_SetColorKey(tmp, (SDL_SRCCOLORKEY | SDL_ACCELERATION_RLE), 0);
	PickupImage = SDL_DisplayFormat(tmp);
	
	if (tmp) SDL_FreeSurface(tmp);
	if (rw) SDL_FreeRW(rw);
#else
	tmp = SDL_LoadBMP(IMAGE_PEN);
	SDL_SetColorKey(tmp, (SDL_SRCCOLORKEY | SDL_ACCELERATION_RLE), SDL_MapRGB(tmp->format, 255, 0, 255));
	PlayerSpritesheets[0] = SDL_DisplayFormat(tmp);
	if (tmp) SDL_FreeSurface(tmp);
	
	tmp = SDL_LoadBMP(IMAGE_PENGUIN_BLACK);
	SDL_SetColorKey(tmp, (SDL_SRCCOLORKEY | SDL_ACCELERATION_RLE), SDL_MapRGB(tmp->format, 255, 0, 255));
	PlayerSpritesheets[1] = SDL_DisplayFormat(tmp);
	if (tmp) SDL_FreeSurface(tmp);

	tmp = SDL_LoadBMP(IMAGE_EGGPLANT);
	SDL_SetColorKey(tmp, (SDL_SRCCOLORKEY | SDL_ACCELERATION_RLE), 0);
	PickupImage = SDL_DisplayFormat(tmp);
	if (tmp) SDL_FreeSurface(tmp);
#endif


#define LOAD_ANIM(_anim, _path, _w, _h, _fps)\
	if (!AnimationLoad(&(_anim), _path, (_w), (_h), (_fps)))\
	{\
		*Continue = false;  *Error = true;\
		printf("SDL_LoadBMP failed: %s\n", SDL_GetError());\
		SDL_ClearError();\
		return;\
	}
	
	LOAD_ANIM(Spark, IMAGE_SPARKS, 4, 4, 20);
	LOAD_ANIM(SparkRed, IMAGE_SPARKS_RED, 4, 4, 20);
	LOAD_ANIM(Tail, IMAGE_TAIL, 21, 21, 15);

	if (!GapSurfacesLoad())
	{
		*Continue = false;  *Error = true;
		printf("SDL_LoadBMP failed: %s\n", SDL_GetError());
		SDL_ClearError();
		return;
	}
	if (!TitleImagesLoad())
	{
		*Continue = false;  *Error = true;
		printf("SDL_LoadBMP failed: %s\n", SDL_GetError());
		SDL_ClearError();
		return;
	}
	if (!BackgroundsLoad(&BG))
	{
		*Continue = false;  *Error = true;
		printf("SDL_LoadBMP failed: %s\n", SDL_GetError());
		SDL_ClearError();
		return;
	}

#ifndef NOSOUND
	
	
#ifdef DREAMCAST
	SoundBeep = snd_sfx_load(SOUND_BEEP_FILE);
	SoundPlayerBounce = snd_sfx_load(SOUND_BOUNCE_FILE);
	SoundStart = snd_sfx_load(SOUND_START_FILE);
	SoundLose = snd_sfx_load(SOUND_LOSE_FILE);
	SoundScore = snd_sfx_load(SOUND_SCORE_FILE);
	SoundLoad();
	sndoggvorbis_start(MUSIC_FILE, -1);
#else
	SoundBeep = Mix_LoadWAV_RW(SDL_RWFromFile(SOUND_BEEP_FILE, "rb"), 1);
	SoundPlayerBounce = Mix_LoadWAV_RW(SDL_RWFromFile(SOUND_BOUNCE_FILE, "rb"), 1);
	SoundStart = Mix_LoadWAV_RW(SDL_RWFromFile(SOUND_START_FILE, "rb"), 1);
	SoundLose = Mix_LoadWAV_RW(SDL_RWFromFile(SOUND_LOSE_FILE, "rb"), 1);
	SoundScore = Mix_LoadWAV_RW(SDL_RWFromFile(SOUND_SCORE_FILE, "rb"), 1);
	SoundLoad();
    music = Mix_LoadMUS(MUSIC_FILE);
#endif
	
	
	if (music == NULL)
	{
		*Continue = false;  *Error = true;
		printf("Mix_LoadMUS failed: %s\n", SDL_GetError());
		SDL_ClearError();
		return;
	}

#endif


#ifdef TTF
	#ifdef EMBED
		rw3 = SDL_RWFromMem(F_FONT, FONT_SIZE);
		font = TTF_OpenFontRW(rw3, 1, 20);
		rw2 = SDL_RWFromMem(F_FONT, FONT_SIZE);
		hsFont = TTF_OpenFontRW(rw2, 1, 16);
	#else
		font = TTF_OpenFont( FILE_FONT, 20);
		hsFont = TTF_OpenFont( FILE_FONT, 16);
	#endif
#endif

	SpaceInit(&space);
	ParticlesInit();
	PickupsInit();

	InitializePlatform();

	// Title screen. (-> title.c)
	MusicSetLoud(false);
#ifndef NOSOUND
#ifndef DREAMCAST
	Mix_PlayMusic(music, -1);
#endif
#endif
	ToTitleScreen(true);
}

void Finalize()
{
	PickupsFree();
	ParticlesFree();
	SpaceFree(&space);
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (PlayerSpritesheets[i]) SDL_FreeSurface(PlayerSpritesheets[i]);
	}
	if (PickupImage) SDL_FreeSurface(PickupImage);
	AnimationFree(&Spark);
	AnimationFree(&SparkRed);
	AnimationFree(&Tail);
	GapSurfacesFree();
	TitleImagesFree();
	BackgroundsFree(&BG);
#ifndef NOSOUND
#ifdef DREAMCAST
	snd_sfx_unload_all();
	sndoggvorbis_stop();
#else
	if (SoundPlayerBounce) Mix_FreeChunk(SoundPlayerBounce);
	if (SoundBeep) Mix_FreeChunk(SoundBeep);
	if (SoundStart) Mix_FreeChunk(SoundStart);
	if (SoundLose) Mix_FreeChunk(SoundLose);
	if (SoundScore)  Mix_FreeChunk(SoundScore);
	if (music) Mix_FreeMusic(music);
#endif
	SoundFree();
#endif

#ifdef TTF
	if (font) TTF_CloseFont(font);
	if (hsFont) TTF_CloseFont(hsFont);
#endif

	HighScoresFree();
	InputFree();
	SDL_Quit();
}
