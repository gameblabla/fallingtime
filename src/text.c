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
#include "text.h"

#ifdef DREAMCAST
#include <kos.h>
#include <string.h>
#endif

#include <stdbool.h>

#include "init.h"
#include "main.h"

#ifndef TTF
	typedef int TTF_Font;
#endif


void TextRenderCentered(
	SDL_Surface *s, TTF_Font *font, const char *text, const int startY,
	const SDL_Color c)
{
#ifdef TTF
	int y = startY;
	for (;;)
	{
		// Render the text line-by-line
		const char *nl = strchr(text, '\n');
		char buf[256];
		if (nl != NULL)
		{
			const size_t len = nl - text;
			strncpy(buf, text, len);
			buf[len] = '\0';
		}
		else
		{
			strcpy(buf, text);
		}

		if (strlen(buf) > 0)
		{
			SDL_Surface *t = TTF_RenderText_Blended(font, buf, c);
			const int x = (SCREEN_WIDTH - t->w) / 2;
			SDL_Rect dest = { (Sint16)x, (Sint16)y, 0, 0 };
			SDL_BlitSurface(t, NULL, s, &dest);

			SDL_FreeSurface(t);
		}
		y += TTF_FontHeight(font);

		if (nl == NULL)
		{
			break;
		}
		text = nl + 1;
	}
#endif
}
