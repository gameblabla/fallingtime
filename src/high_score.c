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
#include "high_score.h"

#include <stdio.h>

#include "cfgpath.h"
#include "init.h"
#include "main.h"
#include "text.h"
#include "utils.h"

#ifdef DREAMCAST
#include <dc/sd.h>
#include <kos/blockdev.h>
#include <ext2/fs_ext2.h>
#include <dc/maple.h>
#include <dc/maple/dreameye.h>
#include <kos/dbgio.h>

#include <kos.h>
#include <zlib/zlib.h> //vmu compression!
#include "dreamcast_icon.h"

kos_blockdev_t sd_dev;
uint8 partition_type;
file_t d;
#endif

#ifdef NSPIRE
#define HIGH_SCORE_FILE "./high.dat.tns"
#else
#define HIGH_SCORE_FILE "falling_time_high_scores"
#endif

#define MAX_HIGH_SCORES 20

CArray HighScores;

void HighScoresInit(void)
{
#ifndef NOCONFIG

	CArrayInit(&HighScores, sizeof(HighScore));

#ifdef DREAMCAST
	FILE *f;
	DC_LoadVMU();
#else

	char buf[MAX_PATH];
	
#ifdef NSPIRE
	snprintf(buf,sizeof(buf), "/documents/ndless/high_ft.tns");
#else
	get_user_config_file(buf, MAX_PATH, HIGH_SCORE_FILE);
#endif

	if (strlen(buf) == 0)
	{
		printf("Error: cannot find config file path\n");
		return;
	}
	FILE *f = fopen(buf, "r");
	if (f == NULL)
	{
		printf("Error: cannot open config file %s\n", buf);
		return;
	}
	while (fgets(buf, sizeof buf, f))
	{
		HighScore hs;
		struct tm tm;
		sscanf(
			buf, "%d %04d-%02d-%02d %02d:%02d:%02d",
			&hs.Score, &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
			&tm.tm_hour, &tm.tm_min, &tm.tm_sec);
		// Time correction
		tm.tm_year -= 1900;
		tm.tm_mon--;
		hs.Time = mktime(&tm);
		CArrayPushBack(&HighScores, &hs);
	}
	fclose(f);
#endif
	
#endif
}


#ifdef DREAMCAST

int DC_LoadVMU(void) 
{
    unsigned long unzipsize;
    uint8* unzipdata;
    vmu_pkg_t pkg;
    FILE* f;
    
    char buf[MAX_PATH];
	snprintf(buf,sizeof(buf), "/ram/fthigh.dat");
	
    // Name of the file to open
    file_t fd;
    if ((fd = fs_open("/vmu/a1/fthigh.dat", O_RDONLY)) == -1)
    {
        printf("error opening VMU A1, not found.\n");
        return -1;
    }
   
    // Remove VMU header
    vmu_pkg_parse(fs_mmap(fd), &pkg);

    // Allocate memory for the uncompressed data
    unzipdata = (uint8 *)malloc(524288); //512KB
    unzipsize = 524288;
    uncompress(unzipdata, &unzipsize, (uint8 *)pkg.data, pkg.data_len);

    // Save buffer into a RAM file
    fs_close(fd);

    file_t fd2;
    if ((fd2 = fs_open(buf, O_WRONLY)) == -1)
    {
        printf("Can't create RAM file from VMU.\n");
        return -1;
    }
    
    fs_write(fd2, unzipdata, unzipsize);	
    fs_close(fd2);

    // Free unused memory
    free(unzipdata);
    
	f = fopen(buf, "r");
	if (f == NULL)
	{
		printf("Error: cannot open config file %s\n", buf);
		return;
	}
	
	CArrayInit(&HighScores, sizeof(HighScore));
		
	while (fgets(buf, sizeof buf, f))
	{
		HighScore hs;
		struct tm tm;
		sscanf(
			buf, "%d %04d-%02d-%02d %02d:%02d:%02d",
			&hs.Score, &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
			&tm.tm_hour, &tm.tm_min, &tm.tm_sec);
		// Time correction
		tm.tm_year -= 1900;
		tm.tm_mon--;
		hs.Time = mktime(&tm);
		CArrayPushBack(&HighScores, &hs);
	}
	
	fclose(f);
	
	//CArrayInit(&HighScores, sizeof(HighScore));

    return 0;
}

int DC_SaveVMU(void) 
{
    vmu_pkg_t pkg;
    uint8 *pkg_out;
    int pkg_size;
    file_t ft;
    FILE* f;
    
    printf("Saving to VMU...\n");
    
    char buf[MAX_PATH];
	snprintf(buf,sizeof(buf), "/ram/fthigh.dat");
	
    f = fopen(buf, "w");
    
	for (int i = 0; i < MIN((int)HighScores.size, MAX_HIGH_SCORES); i++)
	{
		const HighScore *hs = CArrayGet(&HighScores, i);
		struct tm *ptm = gmtime(&hs->Time);
		strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", ptm);
		fprintf(f, "%d %s\n", hs->Score, buf);
	}
	
	fclose(f);
	
	printf("Saved to RAM done...\n");

    // Temporal for reading the file
    file_t file;
    int data_size;
    unsigned long zipsize = 0;
    uint8 *datasave;
    uint8 *zipdata;
   
    // Open file and copy to buffer
	printf("Copy RAM to buffer...\n");
    file = fs_open("/ram/fthigh.dat", O_RDONLY);
    
    data_size = fs_total(file);
    datasave = (uint8 *)malloc(data_size+1);
    fs_read(file, datasave, data_size);
    fs_close(file);
   
    // Allocate some memory for compression
	printf("Allocate some memory for compression...\n");
    zipsize = data_size * 2;
    zipdata = (uint8*)malloc(zipsize);

    // The compressed save
	printf("compress...\n");
    compress(zipdata, &zipsize, datasave, data_size);

	printf("Make the package to the VMU...\n");
    // Make the package to the VMU.
    strcpy(pkg.desc_short, "fallingtime");
    strcpy(pkg.desc_long, "Falling Time");
    strcpy(pkg.app_id, "fallingtime");
    pkg.icon_cnt = 1;
    memcpy((void *)&pkg.icon_pal[0],(void *)&vmu_savestate_pal,32);
    pkg.icon_data = (const uint8*)&vmu_savestate_data;
    pkg.icon_anim_speed = 0;
    pkg.eyecatch_type = VMUPKG_EC_NONE;
    pkg.data_len = zipsize;
    pkg.data = zipdata;
    vmu_pkg_build(&pkg, &pkg_out, &pkg_size);

    // Write at A1 port
	printf("Write at A1 port\n");
    fs_unlink("/vmu/a1/fthigh.dat");
    ft = fs_open("/vmu/a1/fthigh.dat", O_WRONLY);
    if (!ft) {
		printf("SAVING FAILED, NO FILE FOUND\n");
        return -1;
    }
    fs_write(ft, pkg_out, pkg_size);
    fs_close(ft);

	printf("Free unused memory\n");
    // Free unused memory
    free(pkg_out);
    free(datasave);
    free(zipdata);
   
    return 0;
}

#endif


void HighScoresFree(void)
{
	CArrayTerminate(&HighScores);
}

void HighScoresAdd(const int s)
{
	// Find the right position to add the score
	// Scores are in descending order
	HighScore hsNew;
	hsNew.Score = s;
	hsNew.Time = time(NULL);
	bool inserted = false;
	for (int i = 0; i < (int)HighScores.size; i++)
	{
		const HighScore *hs = CArrayGet(&HighScores, i);
		if (hs->Score <= s)
		{
			CArrayInsert(&HighScores, i, &hsNew);
			inserted = true;
			break;
		}
	}
	if (!inserted)
	{
		CArrayPushBack(&HighScores, &hsNew);
	}


#ifndef NOCONFIG

#ifdef DREAMCAST
	FILE *f;
	int sucess;
	char buf[MAX_PATH];
	snprintf(buf,sizeof(buf), "/sd/fallingtime/highscore.dat");

	sucess = DC_SaveVMU();
	
	// Failed to save highscore
	if (sucess != 0)
	{
		printf("SAVING FAILED\n");
		for (int i = 0; i < MIN((int)HighScores.size, MAX_HIGH_SCORES); i++)
		{
			const HighScore *hs = CArrayGet(&HighScores, i);
			struct tm *ptm = gmtime(&hs->Time);
		}	
	}
	
#else

	// Save to file
	char buf[MAX_PATH];
#ifdef NSPIRE
	snprintf(buf,sizeof(buf), "/documents/ndless/high_ft.tns");
#else
	get_user_config_file(buf, MAX_PATH, HIGH_SCORE_FILE);
#endif
	
	if (strlen(buf) == 0)
	{
		printf("Error: cannot find config file path\n");
		return;
	}
	FILE *f = fopen(buf, "w");
	if (f == NULL)
	{
		printf("Error: cannot open config file %s\n", buf);
		return;
	}
	for (int i = 0; i < MIN((int)HighScores.size, MAX_HIGH_SCORES); i++)
	{
		const HighScore *hs = CArrayGet(&HighScores, i);
		struct tm *ptm = gmtime(&hs->Time);
		strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", ptm);
		fprintf(f, "%d %s\n", hs->Score, buf);
	}
	fclose(f);
#endif
	
#endif
}

#ifdef TTF
TTF_Font *hsFont = NULL;
#endif

#define HIGH_SCORE_DISPLAY_DY 20.0f
#define HIGH_SCORE_DISPLAY_DDY 20.0f
#define TEXT_COUNTER_LENGTH 1.5f
#define TEXT_BLUE_LOW 64
#define TEXT_BLUE_HIGH 196

void HighScoreDisplayInit(HighScoreDisplay *h)
{
	h->y = SCREEN_HEIGHT;
	h->dy = -HIGH_SCORE_DISPLAY_DY;
	h->h = 0;
	h->textCounter = 0;
}

void HighScoreDisplayUpdate(HighScoreDisplay *h, const Uint32 ms)
{
	// If display reached top or bottom of screen, scroll the other way
	if (h->y >= SCREEN_HEIGHT - h->h && h->y >= 0)
	{
		h->dy -= HIGH_SCORE_DISPLAY_DDY * ms / 1000;
		if (h->dy < -HIGH_SCORE_DISPLAY_DY) h->dy = -HIGH_SCORE_DISPLAY_DY;
	}
	else if (h->y <= 0 && h->y + h->h <= SCREEN_HEIGHT)
	{
		h->dy += HIGH_SCORE_DISPLAY_DDY * ms / 1000;
		if (h->dy > HIGH_SCORE_DISPLAY_DY) h->dy = HIGH_SCORE_DISPLAY_DY;
	}
	h->y += h->dy * ms / 1000;
	h->textCounter += ms / TEXT_COUNTER_LENGTH / 1000;
	if (h->textCounter > 1.0f) h->textCounter -= 1.0f;
}

void HighScoreDisplayDraw(HighScoreDisplay *h)
{
#ifdef TTF
	char buf[2048];
	strcpy(buf, "High Scores\n\n");
	const bool countHeight = h->h == 0;
	if (countHeight) h->h += TTF_FontHeight(hsFont) * 2;
	for (int i = 0; i < (int)HighScores.size; i++)
	{
		const HighScore *hs = CArrayGet(&HighScores, i);
		char lbuf[256];
		struct tm *ptm = gmtime(&hs->Time);
		char tbuf[256];
		strftime(tbuf, sizeof tbuf, "%Y-%m-%d", ptm);
		sprintf(lbuf, "#%d        %d (%s)\n", i + 1, hs->Score, tbuf);
		strcat(buf, lbuf);
		if (countHeight)
		{
			h->h += TTF_FontHeight(hsFont);
		}
	}
	// Pulsate
	const float scalar =
		h->textCounter > 0.5f ? 1.0f - h->textCounter : h->textCounter;
	SDL_Color c;
	c.b = TEXT_BLUE_LOW + (Uint8)((TEXT_BLUE_HIGH - TEXT_BLUE_LOW) * scalar);
	c.r = c.g = c.b / 2;
	TextRenderCentered(Screen, hsFont, buf, (int)h->y, c);
#endif
}
