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
 * @param early_stop a pointer to a bool to allow the game to end early when false
 * @return a bool [true] when the game is completed or timeouts
 */
bool Flip_start_game(long long timeout, bool* early_stop);

#endif
