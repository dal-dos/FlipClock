OUTFILE = flipclock
OUTDIR = $(HOME)/cmpt433/public/myApps/flipclock

SRCDIR = src

CROSS_COMPILE = arm-linux-gnueabihf-
CC_C = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -Wshadow -pthread

LFLAGS = -L$(HOME)/cmpt433/public/asound_lib_BBB

all: flipclock
flipclock: accelerometer.o utils.o
	$(CC_C) $(CFLAGS) flipclock.c $(SRCDIR)/audio_mixer.c -o $(OUTDIR)/$(OUTFILE) $(LFLAGS) -lasound \
	$(OUTDIR)/accelerometer.o $(OUTDIR)/utils.o

accelerometer.o: $(SRCDIR)/accelerometer.c $(SRCDIR)/accelerometer.h
	$(CC_C) $(CFLAGS) -c $(SRCDIR)/accelerometer.c -o $(OUTDIR)/accelerometer.o

utils.o: $(SRCDIR)/utils.c $(SRCDIR)/utils.h
	$(CC_C) $(CFLAGS) -c $(SRCDIR)/utils.c -o $(OUTDIR)/utils.o

# audio_mixer.o: $(SRCDIR)/audio_mixer.c $(SRCDIR)/audio_mixer.h
# 	$(CC_C) $(CFLAGS) $(SRCDIR)/audio_mixer.c -o $(OUTDIR)/audio_mixer.o $(LFLAGS) -lasound

clean:
	rm $(OUTDIR)/$(OUTFILE) *.o