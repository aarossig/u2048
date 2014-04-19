/*
 * u2048 Header File
 */

#ifndef U2048_H
#define U2048_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "FT800.h"

#define U2048_GAME_SIZE 4

/* Game Structs ***************************************************************/

typedef enum U2048Tile_t {
    U2048Tile_Empty = 0,
    U2048Tile_2 = 2,
    U2048Tile_4 = 4,
    U2048Tile_8 = 8,
    U2048Tile_16 = 16,
    U2048Tile_32 = 32,
    U2048Tile_64 = 64,
    U2048Tile_128 = 128,
    U2048Tile_256 = 256,
    U2048Tile_512 = 512,
    U2048Tile_1024 = 1024,
    U2048Tile_2048 = 2048
} U2048Tile_t;

typedef enum U2048Action_t {
    U2048Action_SwipeRight,
    U2048Action_SwipeDown,
    U2048Action_SwipeUp,
    U2048Action_SwipeLeft
} U2048Action_t;

typedef struct U2048_t {
    uint32_t Score;
    U2048Tile_t Tiles[U2048_GAME_SIZE][U2048_GAME_SIZE];
    FT800_t *ft800;
} U2048_t;

/* Game Functions *************************************************************/

void U2048Init(U2048_t *game, FT800_t *ft800);
void U2048GameRender(U2048_t *game);
void U2048NewTile(U2048_t *game, int x, int y, U2048Tile_t tile);
void U2048Action(U2048_t *game, U2048Action_t action);

U2048Tile_t U2048NextTile(U2048Tile_t tile);

#endif
