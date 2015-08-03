CC = gcc

CFLAGS = -O2 -std=c99 -Wall -DNDEBUG -DTTF -DEMBED $(FFLAGS)
FFLAGS = `sdl-config --cflags --libs` -I./ -I./src -I./src/res -I./src/chipmunk/include
LDFLAGS = -Wl,--as-needed `sdl-config --cflags --libs` -lm -lSDL_mixer -lSDL_ttf -lmodplug -lmikmod
OUTPUT = fallingtime

SOURCES = ./src/platform/general.c ./src/animation.c ./src/bg.c ./src/box.c ./src/camera.c ./src/c_array.c ./src/draw.c ./src/game.c ./src/gap.c ./src/high_score.c ./src/init.c ./src/input.c ./src/main.c ./src/particle.c ./src/pickup.c ./src/player.c ./src/sound.c ./src/space.c ./src/text.c ./src/title.c 
SRC = ./src/chipmunk/src/chipmunk.c ./src/chipmunk/src/cpPolyShape.c ./src/chipmunk/src/cpSpaceStep.c ./src/chipmunk/src/cpGrooveJoint.c ./src/chipmunk/src/cpDampedSpring.c ./src/chipmunk/src/cpArbiter.c ./src/chipmunk/src/cpArray.c ./src/chipmunk/src/cpBBTree.c ./src/chipmunk/src/cpBody.c ./src/chipmunk/src/cpCollision.c ./src/chipmunk/src/cpConstraint.c ./src/chipmunk/src/cpDampedRotarySpring.c ./src/chipmunk/src/cpGearJoint.c ./src/chipmunk/src/cpHashSet.c ./src/chipmunk/src/cpMarch.c ./src/chipmunk/src/cpPinJoint.c ./src/chipmunk/src/cpPivotJoint.c ./src/chipmunk/src/cpPolyline.c ./src/chipmunk/src/cpRatchetJoint.c ./src/chipmunk/src/cpRotaryLimitJoint.c ./src/chipmunk/src/cpShape.c ./src/chipmunk/src/cpSimpleMotor.c ./src/chipmunk/src/cpSlideJoint.c ./src/chipmunk/src/cpSpace.c ./src/chipmunk/src/cpSpaceComponent.c ./src/chipmunk/src/cpSpaceDebug.c ./src/chipmunk/src/cpSpaceHash.c ./src/chipmunk/src/cpSpaceQuery.c ./src/chipmunk/src/cpSpatialIndex.c ./src/chipmunk/src/cpSweep1D.c

OBJS = ${SOURCES:.c=.o} ${SRC:.c=.o}


all: fallingtime

%.o:	%.c
		$(CC) -c -o $@ $? ${CFLAGS}

fallingtime:	${OBJS}
		$(CC) -o $@ $+ ${LDFLAGS}
		
clean:
	rm ${OBJS}
	rm ${OUTPUT}
