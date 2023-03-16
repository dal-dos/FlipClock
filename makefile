OUTFILE = flipclock
OUTDIR = $(HOME)/cmpt433/public/myApps/flipclock

SRCDIR = src
APIDIR = api

CROSS_COMPILE = arm-linux-gnueabihf-
CC_C = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -Wshadow -pthread

LFLAGS = -L$(HOME)/cmpt433/public/asound_lib_BBB

all: flipclock
flipclock: accelerometer.o utils.o time_utils.o clock.o request_handler.o udp_socket.o flip_game.o
	$(CC_C) $(CFLAGS) flipclock.c $(SRCDIR)/audio_mixer.c -o $(OUTDIR)/$(OUTFILE) $(LFLAGS) -lasound \
	$(OUTDIR)/accelerometer.o $(OUTDIR)/utils.o $(OUTDIR)/time_utils.o $(OUTDIR)/clock.o \
	$(OUTDIR)/udp_socket.o $(OUTDIR)/request_handler.o $(OUTDIR)/flip_game.o

accelerometer.o: $(SRCDIR)/accelerometer.c $(SRCDIR)/accelerometer.h
	$(CC_C) $(CFLAGS) -c $(SRCDIR)/accelerometer.c -o $(OUTDIR)/accelerometer.o

utils.o: $(SRCDIR)/utils.c $(SRCDIR)/utils.h
	$(CC_C) $(CFLAGS) -c $(SRCDIR)/utils.c -o $(OUTDIR)/utils.o -lrt

time_utils.o: $(SRCDIR)/time_utils.c $(SRCDIR)/time_utils.h
	$(CC_C) $(CFLAGS) -c $(SRCDIR)/time_utils.c -o $(OUTDIR)/time_utils.o

clock.o: $(SRCDIR)/clock.c $(SRCDIR)/clock.h
	$(CC_C) $(CFLAGS) -c $(SRCDIR)/clock.c -o $(OUTDIR)/clock.o

request_handler.o: $(APIDIR)/request_handler.c $(APIDIR)/request_handler.h
	$(CC_C) $(CFLAGS) -c $(APIDIR)/request_handler.c -o $(OUTDIR)/request_handler.o

udp_socket.o: $(APIDIR)/udp_socket.c $(APIDIR)/udp_socket.h
	$(CC_C) $(CFLAGS) -c $(APIDIR)/udp_socket.c -o $(OUTDIR)/udp_socket.o

flip_game.o: $(SRCDIR)/flip_game.c $(SRCDIR)/flip_game.h
	$(CC_C) $(CFLAGS) -c $(SRCDIR)/flip_game.c -o $(OUTDIR)/flip_game.o

# audio_mixer.o: $(SRCDIR)/audio_mixer.c $(SRCDIR)/audio_mixer.h
# 	$(CC_C) $(CFLAGS) $(SRCDIR)/audio_mixer.c -o $(OUTDIR)/audio_mixer.o $(LFLAGS) -lasound

clean:
	rm $(OUTDIR)/$(OUTFILE) *.o