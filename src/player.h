#pragma once

#include <stdbool.h>

#include <chipmunk/chipmunk.h>
#include <SDL.h>

#ifndef NOSOUND
#include <SDL_mixer.h>
#endif

#ifdef DREAMCAST
#include <kos.h>
#include <string.h>
#endif

#include "animation.h"


typedef struct
{
	int Index;

	// Whether the player is in the game or not
	bool Enabled;
	bool Alive;
	// Once dead, wait this long before respawning
	int RespawnCounter;

	int Score;

	cpBody *Body;
	// Cached positions for drawing only
	float x, y;

	// The last value returned by GetMovement.
	int16_t AccelX;

	// Used to detect rolling for rolling sound
	bool WasOnSurface;

	// Used to detect consecutive fall scoring
	bool ScoredInAir;

	// Used to animate the player rolling
	int Roll;

	// Blink until counter runs out
	int BlinkCounter;
	// Blink when counter reaches zero
	int NextBlinkCounter;

	// Leave tail particles
	int TailCounter;

	SDL_Surface *Sprites;
} Player;

#define MAX_PLAYERS 2
extern Player players[MAX_PLAYERS];

#define PLAYER_SPRITESHEET_WIDTH 35
#define PLAYER_SPRITESHEET_HEIGHT 35

extern SDL_Surface* PlayerSpritesheets[MAX_PLAYERS];
extern Animation Spark;
extern Animation SparkRed;
extern Animation Tail;
#ifndef NOSOUND
#ifdef DREAMCAST
extern sfxhnd_t SoundPlayerBounce;
#else
extern Mix_Chunk* SoundPlayerBounce;
#endif
#endif
extern int SoundPlayerRollChannel;

void PlayerUpdate(Player *player, const Uint32 ms);
void PlayerDraw(const Player *player, const float y);
void PlayerInit(Player *player, const int i, const cpVect pos);
void PlayerReset(Player *player, const int i);

void PlayerScore(Player *player, const bool air);
void PlayerKill(Player *player);
void PlayerRespawn(Player *player, const float x, const float y);
void PlayerRevive(Player *player);

int PlayerAliveCount(void);
int PlayerEnabledCount(void);
