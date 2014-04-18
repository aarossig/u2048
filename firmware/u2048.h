/*
 * u2048 Header File
 */

#ifndef U2048_H
#define U2048_H

#include <stdint.h>

#include "FT800.h"

#define U2048_GAME_SIZE 4

/* Game Structs ***************************************************************/

typedef enum U2048Tile_t {
    U2048Tile_Empty,
    U2048Tile_2,
    U2048Tile_4,
    U2048Tile_8,
    U2048Tile_16,
    U2048Tile_32,
    U2048Tile_64,
    U2048Tile_128,
    U2048Tile_256,
    U2048Tile_512,
    U2048Tile_1024,
    U2048Tile_2048
} U2048Tile_t;

typedef struct U2048_t {
    uint32_t Score;
    U2048Tile_t Tiles[U2048_GAME_SIZE][U2048_GAME_SIZE];
    FT800_t *ft800;
} U2048_t;

/* Game Functions *************************************************************/

void U2048Init(U2048_t *game, FT800_t *ft800);
void U2048GameRender(U2048_t *game);

#endif
