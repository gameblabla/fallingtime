#ifdef NSPIRE
#define SDL_ACCELERATION_RLE 0
#else
#define SDL_ACCELERATION_RLE SDL_RLEACCEL
#endif

#if defined(DREAMCAST)
	#define SOUND_BEEP_FILE "/rd/beep.wav"
	#define SOUND_BOUNCE_FILE "/rd/bounce.wav"
	#define SOUND_START_FILE "/rd/start.wav"
	#define SOUND_LOSE_FILE "/rd/lose.wav"
	#define SOUND_SCORE_FILE "/rd/score.wav"
	#define SOUND_ROLL_FILE "/rd/roll.wav"
	#define MUSIC_FILE "/rd/musicdc.ogg"
	
	#define IMAGE_GAMEOVER "/cd/data/graphics/gameover.bmp"
	#define IMAGE_ANIM "/cd/data/graphics/anim.bmp"
	#define IMAGE_KEYBOARD0 "/cd/data/graphics/keyboard0.bmp"
	#define IMAGE_KEYBOARD1 "/cd/data/graphics/keyboard1.bmp"
	#define IMAGE_FLARE "/cd/data/graphics/flare.bmp"
	#define IMAGE_ICEBERGS "/cd/data/graphics/icebergs.bmp"
	#define IMAGE_STARS "/cd/data/graphics/stars.bmp"
	#define IMAGE_GAMEOVER "/cd/data/graphics/gameover.bmp"
	#define IMAGE_PEN "/cd/data/graphics/penguin_ball.bmp"
	#define IMAGE_PENGUIN_BLACK "/cd/data/graphics/penguin_black.bmp"
	
	#define IMAGE_EGGPLANT "/cd/data/graphics/eggplant.bmp"
	
	#define IMAGE_SPARKS "/cd/data/graphics/sparks.bmp"
	#define IMAGE_SPARKS_RED "/cd/data/graphics/sparks_red.bmp"
	#define IMAGE_TAIL "/cd/data/graphics/tail.bmp"
	
	#define IMAGE_FLOOR0 "/cd/data/graphics/floor0.bmp"
	#define IMAGE_FLOOR1 "/cd/data/graphics/floor1.bmp"
	#define IMAGE_FLOOR2 "/cd/data/graphics/floor2.bmp"
	#define IMAGE_FLOOR3 "/cd/data/graphics/floor3.bmp"
	#define IMAGE_FLOOR4 "/cd/data/graphics/floor4.bmp"
	#define IMAGE_FLOOR5 "/cd/data/graphics/floor5.bmp"
	
	#define IMAGE_ANALOG "/cd/data/graphics/gcw0analog.bmp"
	#define IMAGE_GCW0G "/cd/data/graphics/gcw0g.bmp"
	
	#define FILE_FONT "/cd/data/font.otf"

#elif defined(NSPIRE)

	#define IMAGE_GAMEOVER "./data/gameover.bmp.tns"
	#define IMAGE_ANIM "./data/anim.bmp.tns"
	#define IMAGE_KEYBOARD0 "./data/keyboard0.bmp.tns"
	#define IMAGE_KEYBOARD1 "./data/keyboard1.bmp.tns"
	#define IMAGE_FLARE "./data/flare.bmp.tns"
	#define IMAGE_ICEBERGS "./data/icebergs.bmp.tns"
	#define IMAGE_STARS "./data/stars.bmp.tns"
	#define IMAGE_GAMEOVER "./data/gameover.bmp.tns"
	#define IMAGE_PEN "./data/penguin_ball.bmp.tns"
	#define IMAGE_PENGUIN_BLACK "./data/penguin_black.bmp.tns"
	#define IMAGE_EGGPLANT "./data/eggplant.bmp.tns"
	#define IMAGE_SPARKS "./data/sparks.bmp.tns"
	#define IMAGE_SPARKS_RED "./data/sparks_red.bmp.tns"
	#define IMAGE_TAIL "./data/tail.bmp.tns"
	
	#define IMAGE_FLOOR0 "./data/floor0.bmp.tns"
	#define IMAGE_FLOOR1 "./data/floor1.bmp.tns"
	#define IMAGE_FLOOR2 "./data/floor2.bmp.tns"
	#define IMAGE_FLOOR3 "./data/floor3.bmp.tns"
	#define IMAGE_FLOOR4 "./data/floor4.bmp.tns"
	#define IMAGE_FLOOR5 "./data/floor5.bmp.tns"
	
	#define FILE_FONT "./data/font.otf.tns"
	
#elif __GCW0__

	#define SOUND_BEEP_FILE "data/sounds/beep.ogg"
	#define SOUND_BOUNCE_FILE "data/sounds/bounce.ogg"
	#define SOUND_START_FILE "data/sounds/start.ogg"
	#define SOUND_LOSE_FILE "data/sounds/lose.ogg"
	#define SOUND_SCORE_FILE "data/sounds/score.ogg"
	#define SOUND_ROLL_FILE "data/sounds/roll.ogg"
	#define MUSIC_FILE "data/sounds/music.ogg"
	
	#define IMAGE_GAMEOVER "/data/graphics/gameover.bmp"
	#define IMAGE_ANIM "/data/graphics/anim.bmp"
	#define IMAGE_KEYBOARD0 "/data/graphics/gcw0.bmp"
	#define IMAGE_KEYBOARD1 "/data/graphics/gcw1.bmp"
	#define IMAGE_FLARE "/data/graphics/flare.bmp"
	#define IMAGE_ICEBERGS "/data/graphics/icebergs.bmp"
	#define IMAGE_STARS "/data/graphics/stars.bmp"
	#define IMAGE_GAMEOVER "/data/graphics/gameover.bmp"
	#define IMAGE_PEN "/data/graphics/penguin_ball.bmp"
	#define IMAGE_PENGUIN_BLACK "/data/graphics/penguin_black.bmp"
	#define IMAGE_EGGPLANT "/data/graphics/eggplant.bmp"
	#define IMAGE_SPARKS "/data/graphics/sparks.bmp"
	#define IMAGE_SPARKS_RED "/data/graphics/sparks_red.bmp"
	#define IMAGE_TAIL "/data/graphics/tail.bmp"
	
	#define IMAGE_FLOOR0 "/data/graphics/floor0.bmp"
	#define IMAGE_FLOOR1 "/data/graphics/floor1.bmp"
	#define IMAGE_FLOOR2 "/data/graphics/floor2.bmp"
	#define IMAGE_FLOOR3 "/data/graphics/floor3.bmp"
	#define IMAGE_FLOOR4 "/data/graphics/floor4.bmp"
	#define IMAGE_FLOOR5 "/data/graphics/floor5.bmp"
	
	#define FILE_FONT "/data/font.otf"
	
	#define IMAGE_ANALOG "/data/graphics/gcw0analog.bmp"
	#define IMAGE_GCW0G "/data/graphics/gcw0g.bmp"
	
#else
	#define SOUND_BEEP_FILE "data/sounds/beep.ogg"
	#define SOUND_BOUNCE_FILE "data/sounds/bounce.ogg"
	#define SOUND_START_FILE "data/sounds/start.ogg"
	#define SOUND_LOSE_FILE "data/sounds/lose.ogg"
	#define SOUND_SCORE_FILE "data/sounds/score.ogg"
	#define SOUND_ROLL_FILE "data/sounds/roll.ogg"
	#define MUSIC_FILE "data/sounds/music.ogg"
	
	#define IMAGE_GAMEOVER "data/graphics/gameover.bmp"
	#define IMAGE_ANIM "data/graphics/anim.bmp"
	#define IMAGE_KEYBOARD0 "data/graphics/keyboard0.bmp"
	#define IMAGE_KEYBOARD1 "data/graphics/keyboard1.bmp"
	#define IMAGE_FLARE "data/graphics/flare.bmp"
	#define IMAGE_ICEBERGS "data/graphics/icebergs.bmp"
	#define IMAGE_STARS "data/graphics/stars.bmp"
	#define IMAGE_GAMEOVER "data/graphics/gameover.bmp"
	#define IMAGE_PEN "data/graphics/penguin_ball.bmp"
	#define IMAGE_PENGUIN_BLACK "data/graphics/penguin_black.bmp"
	#define IMAGE_EGGPLANT "data/graphics/eggplant.bmp"
	#define IMAGE_SPARKS "data/graphics/sparks.bmp"
	#define IMAGE_SPARKS_RED "data/graphics/sparks_red.bmp"
	#define IMAGE_TAIL "data/graphics/tail.bmp"
	
	#define IMAGE_FLOOR0 "data/graphics/floor0.bmp"
	#define IMAGE_FLOOR1 "data/graphics/floor1.bmp"
	#define IMAGE_FLOOR2 "data/graphics/floor2.bmp"
	#define IMAGE_FLOOR3 "data/graphics/floor3.bmp"
	#define IMAGE_FLOOR4 "data/graphics/floor4.bmp"
	#define IMAGE_FLOOR5 "data/graphics/floor5.bmp"
	
	#define FILE_FONT "data/font.otf"
#endif
