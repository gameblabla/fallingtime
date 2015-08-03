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
#include "animation.h"
#include "common.h"
#include "init.h"


#ifdef EMBED
#include "sparks.h"
#include "sparks_red.h"
#include "tail.h"
#include "anim.h"
#include "gameover.h"
#endif


bool AnimationLoad(
	Animation *a, const char *filename, const int w, const int h,
	const int frameRate)
{
	SDL_Surface *tmp;
#ifdef EMBED
	SDL_RWops *rw = NULL;
#endif

#ifdef EMBED
	if (strcmp(filename,IMAGE_SPARKS) == 0) rw = SDL_RWFromMem(IMG_SPARKS, SPARKS_SIZE);
	else if (strcmp(filename,IMAGE_SPARKS_RED) == 0) rw = SDL_RWFromMem(IMG_SPARKS_RED, SPARKS_RED_SIZE);
	else if (strcmp(filename,IMAGE_TAIL) == 0) rw = SDL_RWFromMem(IMG_TAIL, TAIL_SIZE);
	else if (strcmp(filename,IMAGE_ANIM) == 0) rw = SDL_RWFromMem(IMG_ANIM, ANIM_SIZE);
	else if (strcmp(filename,IMAGE_GAMEOVER) == 0) rw = SDL_RWFromMem(IMG_GAMEOVER, GAMEOVER_SIZE);
    tmp = SDL_LoadBMP_RW(rw, 0);
    if (rw) SDL_FreeRW(rw);
#else
	tmp = SDL_LoadBMP(filename);
#endif
	
	if (strcmp(filename,IMAGE_GAMEOVER) == 0) 
	{
		SDL_SetColorKey(tmp, (SDL_SRCCOLORKEY | SDL_ACCELERATION_RLE), SDL_MapRGB(tmp->format, 255, 0, 255));
	}
	else 
	{
		SDL_SetColorKey(tmp, (SDL_SRCCOLORKEY | SDL_ACCELERATION_RLE), 0);
	}
	
	a->image = SDL_DisplayFormat(tmp);
	if (tmp) SDL_FreeSurface(tmp);

	if (a->image == NULL) 
	{
		printf("Failure to load %s\n", filename);
		goto bail;
	}
	a->w = w;
	a->h = h;
	a->frame = 0;
	a->frameCounter = 0;
	a->frameRate = frameRate;

	return true;

bail:
	AnimationFree(a);
	return false;
}
void AnimationFree(Animation *a)
{
	SDL_FreeSurface(a->image);
}

bool AnimationUpdate(Animation *a, const Uint32 ms)
{
	a->frameCounter += ms;
	const int msPerFrame = 1000 / a->frameRate;
	if (a->frameCounter > msPerFrame)
	{
		a->frameCounter -= msPerFrame;
		a->frame++;
		if (a->frame == (a->image->w / a->w) * (a->image->h / a->h))
		{
			a->frame = 0;
			return true;
		}
	}
	return false;
}

void AnimationDraw(const Animation *a, SDL_Surface *screen, const int x, const int y)
{
	const int stride = a->image->w / a->w;
	SDL_Rect src = {
		(Sint16)((a->frame % stride) * a->w),
		(Sint16)((a->frame / stride) * a->h),
		(Sint16)a->w, (Sint16)a->h
	};
	SDL_Rect dest = { (Sint16)(x - a->w / 2), (Sint16)(y - a->h / 2), 0, 0 };
	SDL_BlitSurface(a->image, &src, screen, &dest);
}
void AnimationDrawUpperCenter(const Animation *a, SDL_Surface *screen)
{
	AnimationDraw(a, screen, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4);
}
