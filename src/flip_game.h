// flip_game.h
#ifndef _FLIP_GAME_H_
#define _FLIP_GAME_H_

#include <stdbool.h>

// // start/stop the flip game thread
// void Flip_start_thread(void);
// void Flip_stop_thread(void);

/**
 * Starts a new flip game
 *
 * @param timeout a long long (milliseconds) representing the time limit of the game
 * @return a bool [true] when the game is completed or timeouts
 */
bool Flip_start_game(long long timeout);

#endif
