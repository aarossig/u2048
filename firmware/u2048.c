/*
 * u2048 Implementation
 */

#include "u2048.h"

/* Constants ******************************************************************/

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 272

#define U2048_TILE_SIZE 54
#define U2048_SIZE 272
#define U2048_XPOS 0
#define U2048_YPOS 0
#define U2048_TILE_SPACING ((U2048_SIZE - (U2048_GAME_SIZE * U2048_TILE_SIZE)) \
                           / (U2048_GAME_SIZE + 1))

const FT800Color_t ColorWhite = { .Red = 255, .Green = 255, .Blue = 255 };
const FT800Color_t ColorBoard = { .Red = 187, .Green = 173, .Blue = 160 };
const FT800Color_t ColorEmpty = { .Red = 119, .Green = 110, .Blue = 101 };
const FT800Color_t ColorTwo = { .Red = 238, .Green = 228, .Blue = 218 };
const FT800Color_t ColorDefault = { .Red = 0, .Green = 255, .Blue = 255 };

/* Render Functions ***********************************************************/

FT800Color_t U2048GetTileColor(U2048Tile_t tile);
void U2048RenderStart(U2048_t *game);
void U2048RenderBoard(U2048_t *game);
void U2048RenderTiles(U2048_t *game);
void U2048RenderFinish(U2048_t *game);

void U2048NewTile(U2048_t *game, int x, int y, U2048Tile_t tile);

/* Game Functions *************************************************************/


void U2048Init(U2048_t *game, FT800_t *ft800)
{
    game->ft800 = ft800;
    game->Score = 0;
    
    for(int i = 0; i < U2048_GAME_SIZE; i++)
    {
        for(int j = 0; j < U2048_GAME_SIZE; j++)
        {
            game->Tiles[i][j] = U2048Tile_Empty;
        }
    }
}

void U2048GameRender(U2048_t *game)
{
    U2048RenderStart(game);
    U2048RenderBoard(game);
    U2048RenderTiles(game);
    U2048RenderFinish(game);
}

void U2048NewTile(U2048_t *game, int x, int y, U2048Tile_t tile)
{
    for(int i = 0; i < (U2048_TILE_SIZE / 2); i++)
    {
        U2048RenderStart(game);
        U2048RenderBoard(game);
        U2048RenderTiles(game);
        
        FT800Color_t color = U2048GetTileColor(tile);
        FT800DlRgb(game->ft800, color);
        
        FT800Point_t p1 = {
            .X = ((x + 1) * U2048_TILE_SPACING) + (x * U2048_TILE_SIZE)
                + (U2048_TILE_SIZE / 2) - i - 1,
            .Y = ((y + 1) * U2048_TILE_SPACING) + (y * U2048_TILE_SIZE)
                + (U2048_TILE_SIZE / 2) - i - 1,
        };

        FT800Point_t p2 = {
            .X = ((x + 1) * U2048_TILE_SPACING) + (x * U2048_TILE_SIZE)
                + (U2048_TILE_SIZE / 2) + i + 1,
            .Y = ((y + 1) * U2048_TILE_SPACING) + (y * U2048_TILE_SIZE)
                + (U2048_TILE_SIZE / 2) + i + 1,
        };

        FT800DrawRectangle(game->ft800, p1, p2);
        
        U2048RenderFinish(game);
    }

    game->Tiles[x][y] = tile;
}

/* Render Functions ***********************************************************/

FT800Color_t U2048GetTileColor(U2048Tile_t tile)
{
    switch(tile)
    {
        case U2048Tile_Empty:
            return ColorEmpty;
        case U2048Tile_2:
            return ColorTwo;
        default:
            return ColorDefault;
    }
}

void U2048RenderStart(U2048_t *game)
{
    FT800CmdNewDisplayList(game->ft800);
    FT800DlClearCSTBuffers(game->ft800, true, true, true);
}

void U2048RenderBoard(U2048_t *game)
{
    FT800Point_t p1 = {
        .X = U2048_XPOS,
        .Y = U2048_YPOS
    };

    FT800Point_t p2 = {
        .X = U2048_XPOS + U2048_SIZE,
        .Y = U2048_YPOS + U2048_SIZE
    };
    
    FT800DlRgb(game->ft800, ColorBoard);
    FT800DrawRectangle(game->ft800, p1, p2);
}

void U2048RenderTiles(U2048_t *game)
{
    FT800Color_t currentColor = { .Red = 0, .Green = 0, .Blue = 0 };
    
    for(int i = 0; i < U2048_GAME_SIZE; i++)
    {
        for(int j = 0; j < U2048_GAME_SIZE; j++)
        {
            FT800Point_t p1 = {
                .X = ((i + 1) * U2048_TILE_SPACING) + (i * U2048_TILE_SIZE),
                .Y = ((j + 1) * U2048_TILE_SPACING) + (j * U2048_TILE_SIZE),
            };

            FT800Point_t p2 = {
                .X = p1.X + U2048_TILE_SIZE,
                .Y = p1.Y + U2048_TILE_SIZE
            };
            
            FT800Color_t newColor = U2048GetTileColor(game->Tiles[i][j]);
            
            if(!FT800ColorsEqual(newColor, currentColor))
            {
                FT800DlRgb(game->ft800, newColor);
            }
            
            FT800DrawRectangle(game->ft800, p1, p2);
            
            if(game->Tiles[i][j] != U2048Tile_Empty)
            {
                char str[5];
                sprintf(str, "%d", game->Tiles[i][j]);

                p1.X += U2048_TILE_SIZE / 2;
                p1.Y += U2048_TILE_SIZE / 2;

                FT800DlRgb(game->ft800, ColorWhite);
                
                FT800CmdDrawText(game->ft800, p1, FT800Font_AntiAliased3,
                        FT800Option_CenterX | FT800Option_CenterY,
                        str, strlen(str));
            }
        }
    }
}

void U2048RenderFinish(U2048_t *game)
{
    FT800DlEnd(game->ft800);
    FT800CmdSwapDisplayList(game->ft800);
    FT800CmdFlush(game->ft800);
}



