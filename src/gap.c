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
#include "gap.h"
#include <stdlib.h>
#include <math.h>

#include "common.h"

#include "box.h"
#include "game.h"
#include "main.h"
#include "pickup.h"

#ifdef EMBED
#include "floor0.h"
#include "floor1.h"
#include "floor2.h"
#include "floor3.h"
#include "floor4.h"
#include "floor5.h"
#endif

#define GAP_SPRITE_WIDTH 318
#define GAP_SPRITE_HEIGHT 15

SDL_Surface *GapSurfaces[6];

static int compareFloat(const void *a, const void *b);
void GapInit(struct Gap* gap, const float w, const float y)
{
	// Randomly generate some gaps, and place blocks around them
	float gapXs[MAX_GAPS];
	for (int i = 0; i < MAX_GAPS; i++)
	{
		gapXs[i] = w / 2 + (float)rand() / (float)RAND_MAX * (FIELD_WIDTH - w);
	}
	qsort(gapXs, MAX_GAPS, sizeof gapXs[0], compareFloat);
	// Merge gaps if they are too close
	for (int i = 1; i < MAX_GAPS; i++)
	{
		if (gapXs[i] - gapXs[i - 1] < w + MIN_BLOCK_WIDTH)
		{
			gapXs[i] = (gapXs[i] + gapXs[i - 1]) / 2;
			gapXs[i - 1] = 0;
		}
	}
	// Generate blocks
	CArrayInit(&gap->blocks, sizeof(Block));
	Block b;
	float left = 0;
	for (int i = 0; i < MAX_GAPS; i++)
	{
		if (gapXs[i] == 0) continue;
		BlockInit(&b, left, y, gapXs[i] - w / 2 - left);
		CArrayPushBack(&gap->blocks, &b);
		left = gapXs[i] + w / 2;
	}
	// Add last block
	BlockInit(&b, left, y, FIELD_WIDTH - left);
	CArrayPushBack(&gap->blocks, &b);

	// Randomly add a pickup above a block
	if (rand() > (RAND_MAX / 2))
	{
		const Block *bl = CArrayGet(&gap->blocks, rand() % gap->blocks.size);
		const cpVect pos = cpBodyGetPosition(bl->Body);
		PickupsAdd((float)pos.x, (float)pos.y + bl->H / 2);
	}

	memset(gap->Passed, 0, sizeof gap->Passed);
	gap->Y = y;
}
static int compareFloat(const void *a, const void *b)
{
	const float fa = *(const float *)a;
	const float fb = *(const float *)b;
	return (fa > fb) - (fa < fb);
}
void GapRemove(struct Gap* gap)
{
	for (int i = 0; i < (int)gap->blocks.size; i++)
	{
		BlockRemove(CArrayGet(&gap->blocks, i));
	}
	CArrayTerminate(&gap->blocks);
}

void GapDraw(const struct Gap* gap, const float y)
{
	for (int i = 0; i < (int)gap->blocks.size; i++)
	{
		BlockDraw(CArrayGet(&gap->blocks, i), y);
	}
}

float GapBottom(const struct Gap* gap)
{
	return gap->Y - GAP_HEIGHT;
}

bool GapSurfacesLoad(void)
{
	SDL_Surface *tmp;
#ifdef EMBED
	SDL_RWops *rw;
#endif
	
	for (int i = 0; i < 6; i++)
	{
#ifdef EMBED
		if (i == 0) rw = SDL_RWFromMem(IMG_FLOOR0, FLOOR0_SIZE);
		else if (i == 1) rw = SDL_RWFromMem(IMG_FLOOR1, FLOOR1_SIZE);
		else if (i == 2) rw = SDL_RWFromMem(IMG_FLOOR2, FLOOR2_SIZE);
		else if (i == 3) rw = SDL_RWFromMem(IMG_FLOOR3, FLOOR3_SIZE);
		else if (i == 4) rw = SDL_RWFromMem(IMG_FLOOR4, FLOOR4_SIZE);
		else if (i == 5) rw = SDL_RWFromMem(IMG_FLOOR5, FLOOR5_SIZE);
		tmp = SDL_LoadBMP_RW(rw, 0);
		SDL_FreeRW(rw);
#else
		if (i == 0) tmp = SDL_LoadBMP(IMAGE_FLOOR0);
		else if (i == 1) tmp = SDL_LoadBMP(IMAGE_FLOOR1);
		else if (i == 2) tmp = SDL_LoadBMP(IMAGE_FLOOR2);
		else if (i == 3) tmp = SDL_LoadBMP(IMAGE_FLOOR3);
		else if (i == 4) tmp = SDL_LoadBMP(IMAGE_FLOOR4);
		else if (i == 5) tmp = SDL_LoadBMP(IMAGE_FLOOR5);
#endif
		
		SDL_SetColorKey(tmp, (SDL_SRCCOLORKEY | SDL_ACCELERATION_RLE), 0);
		GapSurfaces[i] = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
		
		if (GapSurfaces[i] == NULL)
		{
			return false;
		}
	}
	return true;
}
void GapSurfacesFree(void)
{
	for (int i = 0; i < 6; i++)
	{
		SDL_FreeSurface(GapSurfaces[i]);
	}
}
