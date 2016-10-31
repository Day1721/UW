/** @file
    Interface of game engine.

 */

#ifndef ENGINE_H
#define ENGINE_H

/**
 * Initializes a game. Needed before first INIT.
 */
void start_game();

/**
 * Frees memory. Needed after finishing game.
 */
void end_game();

/**
 * Initializes a game with size of a board, number of rounds and positions of kings.
 */
int init(int n, int k, int p, int x1, int y1, int x2, int y2);

/**
 * Makes a move.
 * @param[in] x1 Column number before a move.
 * @param[in] y1 Row number before a move.
 * @param[in] x2 Column number after a move.
 * @param[in] y2 Row number before a move.
 * @return 42, if there is input error, instead 0.
 */
int move(int x1, int y1, int x2, int y2);

/**
 * Produces Knight from Peasant.
 * @param[in] x1 Column number of a Peasant.
 * @param[in] y1 Row number of a Peasant.
 * @param[in] x2 Column number of new Knight.
 * @param[in] y2 Row number of new Knight.
 * @return 42, if there is input error, instead 0.
 */
int produce_knight(int x1, int y1, int x2, int y2);

/**
 * Produces new Peasant from existing Peasant.
 * @param[in] x1 Column number of an old Peasant.
 * @param[in] y1 Row number of an old Peasant.
 * @param[in] x2 Column number of a new Peasant.
 * @param[in] y2 Row number of a new Peasant.
 * @return 42, if there is input error, instead 0.
 */
int produce_peasant(int x1, int y1, int x2, int y2);

/**
 * Ending the turn
 * @return 42, if it's time to end the game, instead 0.
 */
int end_turn();

/* *
 * Prints (into stdout) top-left corner of the board of size m x m where m = min(n,10).
 */
//void print_topleft();

int make_turn();

#endif /* ENGINE_H */
