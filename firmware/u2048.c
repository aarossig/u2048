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

/* Game Colors ****************************************************************/

const FT800Color_t ColorBoard = { .Red = 187, .Green = 173, .Blue = 160 };

const FT800Color_t ColorEmpty = { .Red = 204, .Green = 192, .Blue = 179 };
const FT800Color_t Color2 = { .Red = 238, .Green = 228, .Blue = 218 };
const FT800Color_t Color4 = { .Red = 237, .Green = 224, .Blue = 200 };
const FT800Color_t Color8 = { .Red = 242, .Green = 177, .Blue = 121 };
const FT800Color_t Color16 = { .Red = 245, .Green = 149, .Blue = 99 };
const FT800Color_t Color32 = { .Red = 246, .Green = 124, .Blue = 95 };
const FT800Color_t Color64 = { .Red = 246, .Green = 94, .Blue = 59 };
const FT800Color_t Color128 = { .Red = 237, .Green = 207, .Blue = 114 };
const FT800Color_t Color256 = { .Red = 237, .Green = 204, .Blue = 97 };
const FT800Color_t Color512 = { .Red = 237, .Green = 200, .Blue = 80 };
const FT800Color_t Color1024 = { .Red = 237, .Green = 197, .Blue = 63 };
const FT800Color_t Color2048 = { .Red = 237, .Green = 194, .Blue = 46 };

const FT800Color_t ColorTextDark = { .Red = 119, .Green = 110, .Blue = 101 };
const FT800Color_t ColorTextBright = { .Red = 249, .Green = 246, .Blue = 242 };

const FT800Color_t ColorDefault = { .Red = 0, .Green = 255, .Blue = 255 };

/* Render Functions ***********************************************************/

FT800Color_t U2048GetTileColor(U2048Tile_t tile);
FT800Color_t U2048GetTextColor(U2048Tile_t tile);
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
            return Color2;
        case U2048Tile_4:
            return Color4;
        case U2048Tile_8:
            return Color8;
        case U2048Tile_16:
            return Color16;
        case U2048Tile_32:
            return Color32;
        case U2048Tile_64:
            return Color64;
        case U2048Tile_128:
            return Color128;
        case U2048Tile_256:
            return Color256;
        case U2048Tile_512:
            return Color512;
        case U2048Tile_1024:
            return Color1024;
        case U2048Tile_2048:
            return Color2048;
        default:
            return ColorDefault;
    }
}

FT800Color_t U2048GetTextColor(U2048Tile_t tile)
{
    if(tile < U2048Tile_8)
    {
        return ColorTextDark;
    }
    else
    {
        return ColorTextBright;
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
                currentColor = newColor;
                FT800DlRgb(game->ft800, newColor);
            }
            
            FT800DrawRectangle(game->ft800, p1, p2);
            
            if(game->Tiles[i][j] != U2048Tile_Empty)
            {
                char str[5];
                sprintf(str, "%d", game->Tiles[i][j]);

                p1.X += U2048_TILE_SIZE / 2;
                p1.Y += U2048_TILE_SIZE / 2;
                
                FT800Color_t textColor = U2048GetTextColor(game->Tiles[i][j]);
                
                if(!FT800ColorsEqual(textColor, currentColor))
                {
                    currentColor = textColor;
                    FT800DlRgb(game->ft800, textColor);
                }
                
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



