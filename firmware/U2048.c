/*
 * u2048 Implementation
 */

#include "u2048.h"

/* Game Functions *************************************************************/

void U2048Init(U2048_t *game)
{
    game->Score = 0;
    
    for(int i = 0; i < U2048_SIZE; i++)
    {
        for(int j = 0; j < U2048_SIZE; j++)
        {
            game->Tiles[i][j] = U2048Tile_Empty;
        }
    }
}

