OUTFILE = flipclock
OUTDIR = $(HOME)/cmpt433/public/myApps/flipclock

SRCDIR = src
APIDIR = api

CROSS_COMPILE = arm-linux-gnueabihf-
CC_C = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -Wshadow -pthread

LFLAGS = -L$(HOME)/cmpt433/public/asound_lib_BBB

all: flipclock server
flipclock: accelerometer.o utils.o time_utils.o clock.o request_handler.o udp_socket.o flip_game.o alarm.o
	$(CC_C) $(CFLAGS) flipclock.c $(SRCDIR)/audio_mixer.c -o $(OUTDIR)/$(OUTFILE) $(LFLAGS) -lasound \
	$(OUTDIR)/accelerometer.o $(OUTDIR)/utils.o $(OUTDIR)/time_utils.o $(OUTDIR)/clock.o \
	$(OUTDIR)/udp_socket.o $(OUTDIR)/request_handler.o $(OUTDIR)/flip_game.o $(OUTDIR)/alarm.o

%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h
	$(CC_C) $(CFLAGS) -c $< -o $(OUTDIR)/$@

%.o: $(APIDIR)/%.c $(APIDIR)/%.h
	$(CC_C) $(CFLAGS) -c $< -o $(OUTDIR)/$@

# audio_mixer.o: $(SRCDIR)/audio_mixer.c $(SRCDIR)/audio_mixer.h
# 	$(CC_C) $(CFLAGS) $(SRCDIR)/audio_mixer.c -o $(OUTDIR)/audio_mixer.o $(LFLAGS) -lasound

server:
	mkdir -p $(OUTDIR)/alarm-clock-server-copy/
	cp -r alarm-clock-server/* $(OUTDIR)/alarm-clock-server-copy/ 

clean:
	rm -f $(OUTDIR)/$(OUTFILE) $(OUTDIR)/*.o