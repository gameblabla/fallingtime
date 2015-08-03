/*
 * Ativayeban, title screen code file
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
#include "title.h"

#ifdef EMBED
#include "keyboard0.h"
#include "keyboard1.h"
#endif

#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdlib.h>

#include "common.h"

#include "animation.h"
#include "box.h"
#include "main.h"
#include "high_score.h"
#include "init.h"
#include "input.h"
#include "particle.h"
#include "platform.h"
#include "player.h"
#include "sound.h"
#include "space.h"
#include "text.h"
#include "game.h"
#include "bg.h"
#include "sys_specifics.h"

static bool Start = false;
static char WelcomeMessage[256];
static int winnerIndices[MAX_PLAYERS];
static int winners = 0;
static Animation TitleAnim;
static Animation GameOverAnim;
static HighScoreDisplay HSD;
SDL_Surface *ControlSurfaces[MAX_PLAYERS];
#if defined(__GCW0__)
SDL_Surface *ControlSurface0Analog = NULL;
SDL_Surface *ControlSurface0G = NULL;
#endif

static Block blocks[MAX_PLAYERS];
#define BLOCK_WIDTH (FIELD_WIDTH / MAX_PLAYERS * 0.25f)
#define BLOCK_Y (FIELD_HEIGHT * 0.5f)

// Countdown to start the game automatically; starts when a player is enabled
static int countdownMs = -1;
#ifdef NSPIRE
#define COUNTDOWN_START_MS 999
#else
#define COUNTDOWN_START_MS 3999
#endif

bool playersEnabled[MAX_PLAYERS];


static void TitleScreenEnd(void);
void TitleScreenGatherInput(bool* Continue)
{
#ifdef DREAMCAST
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		players[i].AccelX = GetMovement(i);
	}
#else

	SDL_Event ev;
	while (SDL_PollEvent(&ev))
	{
		if (IsExitGameEvent(&ev))
		{
			*Continue = false;
			TitleScreenEnd();
			return;
		}
		InputOnEvent(&ev);
		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			players[i].AccelX = GetMovement(i);
		}
#if defined(__GCW0__) 
		// Enable/disable G-Sensor based on up/down
		if (ev.type == SDL_KEYUP &&
			(ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_DOWN))
		{
			InputSwitchJoystick(ev.key.keysym.sym == SDLK_UP ? -1 : 1);
			SoundPlay(SoundScore, 1.0);
		}
#endif
	}
#endif	
}
static void TitleScreenEnd(void)
{
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		BlockRemove(&blocks[i]);
		// Kill players that have not been enabled
		players[i].Enabled = playersEnabled[i];
		players[i].Alive = playersEnabled[i];
	}
}

void TitleScreenDoLogic(bool* Continue, bool* Error, Uint32 Milliseconds)
{
	(void)Continue;
	(void)Error;
	cpSpaceStep(space.Space, Milliseconds * 0.001);
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		PlayerUpdate(&players[i], Milliseconds);

		// Check which players have fallen below their start pads
		cpVect pos = cpBodyGetPosition(players[i].Body);
		if (pos.y < BLOCK_Y)
		{
			if (!playersEnabled[i])
			{
				// New player entered
				countdownMs = COUNTDOWN_START_MS;
				SoundPlay(SoundStart, 1.0);
			}
			playersEnabled[i] = true;
		}
	}

	if (countdownMs >= 0)
	{
		const int countdownMsNext = countdownMs - Milliseconds;
		// Play a beep every second
		if ((countdownMs / 1000) > (countdownMsNext / 1000))
		{
			SoundPlay(SoundBeep, 1.0);
		}
		// Start game if counted down to zero
		if (countdownMsNext <= 0)
		{
			TitleScreenEnd();
			ToGame();
			return;
		}
		countdownMs = countdownMsNext;
	}

	Animation *a = Start ? &TitleAnim : &GameOverAnim;
	AnimationUpdate(a, Milliseconds);

	HighScoreDisplayUpdate(&HSD, Milliseconds);
}

static SDL_Surface *GetControlSurface(const int i);
static void DrawTitleImg(void);
void TitleScreenOutputFrame(void)
{
	DrawBackground(&BG, 0);

	HighScoreDisplayDraw(&HSD);

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		SDL_Surface *s = GetControlSurface(i);
		SDL_Rect dest =
		{
			(Sint16)(
				SCREEN_X((i + 1) * FIELD_WIDTH / (MAX_PLAYERS + 1)) -
				s->w / 2),
			(Sint16)((SCREEN_HEIGHT - s->h) / 2 - SCREEN_X(PLAYER_RADIUS)),
			0,
			0
		};
		SDL_BlitSurface(s, NULL, Screen, &dest);
	}

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		PlayerDraw(&players[i], 0);
	}

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		BlockDraw(&blocks[i], 0);
	}

	DrawTitleImg();
	// Draw player icons if winners
	if (!Start)
	{
		const int left =
			(SCREEN_WIDTH - PLAYER_SPRITESHEET_WIDTH * winners) / 2;
		for (int i = 0; i < winners; i++)
		{
			const int playerIndex = winnerIndices[i];
			SDL_Rect src = {
				0, 0, PLAYER_SPRITESHEET_WIDTH, PLAYER_SPRITESHEET_HEIGHT
			};
			SDL_Rect dest =
			{
				(Sint16)(left + i * PLAYER_SPRITESHEET_WIDTH),
				(Sint16)(SCREEN_HEIGHT * 0.66f),
				0, 0
			};
			SDL_BlitSurface(
				PlayerSpritesheets[playerIndex], &src, Screen, &dest);
		}
	}
	SDL_Color c = { 177, 177, 177, 255 };
#ifdef TTF
	TextRenderCentered(
		Screen, font, WelcomeMessage, (int)(SCREEN_HEIGHT * 0.75f), c);
#endif

	SDL_Flip(Screen);
}
static SDL_Surface *GetControlSurface(const int i)
{
	SDL_Surface *s = ControlSurfaces[i];
#if defined(__GCW0__)
	if (i == 0)
	{
		switch (JoystickIndex)
		{
		case 0:
			s = ControlSurface0Analog;
			break;
		case 1:
			s = ControlSurface0G;
			break;
		default:
			// Do nothing
			break;
		}
	}
#endif
	return s;
}
static void DrawTitleImg(void)
{
	const Animation *a = Start ? &TitleAnim : &GameOverAnim;
	AnimationDrawUpperCenter(a, Screen);
}

void ToTitleScreen(const bool start)
{
	countdownMs = -1;
	ResetMovement();
	MusicSetLoud(false);
	BackgroundsInit(&BG);
	Start = start;
	if (Start)
	{
#ifndef DREAMCAST
		sprintf(
			WelcomeMessage,
			"%s to pause\n%s to exit",
			GetPausePrompt(), GetExitGamePrompt());
#endif
	}
	else
	{
		// Find out the result of the game
		int maxScore = 0;
		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			if (players[i].Score > maxScore) maxScore = players[i].Score;
		}
		winners = 0;
		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			if (!players[i].Enabled) continue;
			if (players[i].Score == maxScore)
			{
				winnerIndices[winners] = i;
				winners++;
			}
		}
		if (PlayerEnabledCount() == 1)
		{
			sprintf(
				WelcomeMessage,
#ifdef DREAMCAST
				"Your score was %d!",
#else
				"Your score was %d!\n%s to exit",
#endif
				maxScore, GetExitGamePrompt());
		}
		else if (winners == 1)
		{
			sprintf(
				WelcomeMessage,
				"Wins with score %d!\n%s to exit",
				maxScore, GetExitGamePrompt());
		}
		else
		{
			sprintf(
				WelcomeMessage,
				"Tied with score %d!\n%s to exit",
				maxScore, GetExitGamePrompt());
		}
		HighScoresAdd(maxScore);
	}

	HighScoreDisplayInit(&HSD);

	CArrayClear(&Particles);
	SpaceReset(&space);
	// Add bottom edge so we don't fall through
	SpaceAddBottomEdge(&space);

	// Initialise players here
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		PlayerInit(&players[i], i, cpv(
			(i + 1) * FIELD_WIDTH / (MAX_PLAYERS + 1),
			FIELD_HEIGHT * 0.75f));
		playersEnabled[i] = false;
	}

	// Add platforms for players to jump off
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		BlockInit(
			&blocks[i],
			(i + 1) * FIELD_WIDTH / (MAX_PLAYERS + 1) - BLOCK_WIDTH / 2,
			BLOCK_Y,
			BLOCK_WIDTH);
	}

	GatherInput = TitleScreenGatherInput;
	DoLogic     = TitleScreenDoLogic;
	OutputFrame = TitleScreenOutputFrame;
}

bool TitleImagesLoad(void)
{
#ifdef EMBED
	SDL_RWops *rw;
#endif
	SDL_Surface *tmp;

	if (!AnimationLoad(&TitleAnim, IMAGE_ANIM, 169, 40, 12))
	{
		return false;
	}
	
	if (!AnimationLoad(&GameOverAnim, IMAGE_GAMEOVER, 131, 40, 3))
	{
		return false;
	}
	for (int i = 0; i < MAX_PLAYERS; i++)
	{

#ifdef EMBED
		if (i == 0) rw = SDL_RWFromMem(IMG_KEYBOARD0, KEYBOARD0_SIZE);
		else rw = SDL_RWFromMem(IMG_KEYBOARD1, KEYBOARD1_SIZE);
		tmp = SDL_LoadBMP_RW(rw, 0);
		SDL_FreeRW(rw);
#else
		if (i == 0) tmp = SDL_LoadBMP(IMAGE_KEYBOARD0);
		else tmp = SDL_LoadBMP(IMAGE_KEYBOARD1);
#endif
		SDL_SetColorKey(tmp, (SDL_SRCCOLORKEY | SDL_ACCELERATION_RLE), 0);
		ControlSurfaces[i] = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);

		if (ControlSurfaces[i] == NULL)
		{
			return false;
		}
	}
#if defined(__GCW0__)
		tmp = SDL_LoadBMP(IMAGE_ANALOG);
		SDL_SetColorKey(tmp, (SDL_SRCCOLORKEY | SDL_ACCELERATION_RLE), 0);
		ControlSurface0Analog = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);

		tmp = SDL_LoadBMP(IMAGE_GCW0G);
		SDL_SetColorKey(tmp, (SDL_SRCCOLORKEY | SDL_ACCELERATION_RLE), 0);
		ControlSurface0G = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
#endif
	return true;
}
void TitleImagesFree(void)
{
	AnimationFree(&TitleAnim);
	AnimationFree(&GameOverAnim);
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (ControlSurfaces[i]) SDL_FreeSurface(ControlSurfaces[i]);
	}
#if defined(__GCW0__)
	SDL_FreeSurface(ControlSurface0Analog);
	SDL_FreeSurface(ControlSurface0G);
#endif
}
