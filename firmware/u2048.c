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
#define U2048_TILE_DELTA (U2048_TILE_SIZE + U2048_TILE_SPACING)
#define U2048_TILE_COUNT (U2048_GAME_SIZE - 1)

#define U2048_NEW_TILE_STEP 2
#define U2048_TILE_MOVE_STEP 8

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
void U2048RenderTile(U2048_t *game,
    FT800Point_t p1, FT800Point_t p2, U2048Tile_t tile);
void U2048GetTileCorners(FT800Point_t *p1, FT800Point_t *p2, int x, int y);
void U2048RenderTileString(U2048_t *game, U2048Tile_t tile, FT800Point_t p);
void U2048RenderFinish(U2048_t *game);

void U2048ActionRight(U2048_t *game);

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
    for(int i = 0; i < (U2048_TILE_SIZE / 2); i += U2048_NEW_TILE_STEP)
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
        
        if(i == ((U2048_TILE_SIZE / 2)) - 1)
        {
            U2048RenderTileString(game, tile, p1);
        }
        
        U2048RenderFinish(game);
    }
    
    game->Tiles[x][y] = tile;
}

void U2048JustifyRight(U2048_t *game)
{
    for(int y = 0; y < U2048_GAME_SIZE; y++)
    {
        for(int x = U2048_TILE_COUNT; x > 0; x--)
        {
            if(game->Tiles[x][y] == U2048Tile_Empty)
            {
                bool emptyToEnd = true;
                for(int i = 0; i < x; i++)
                {
                    if(game->Tiles[i][y] != U2048Tile_Empty)
                    {
                        emptyToEnd = false;
                        break;
                    }
                }

                if(emptyToEnd)
                    break;
                
                for(int f = 0; f < U2048_TILE_DELTA; f += U2048_TILE_MOVE_STEP)
                {
                    U2048RenderStart(game);
                    U2048RenderBoard(game);
                    
                    for(int i = U2048_TILE_COUNT; i >= 0; i--)
                    {
                        for(int j = 0; j < U2048_GAME_SIZE; j++)
                        {
                            FT800Point_t p1;
                            FT800Point_t p2;
                            
                            U2048GetTileCorners(&p1, &p2, i, j);
                            
                            if(j == y && i < x)
                            {
                                p1.X += f;
                                p2.X += f;
                            }
                            
                            U2048RenderTile(game, p1, p2, game->Tiles[i][j]);
                        }
                    }
                    
                    U2048RenderFinish(game);

                    if(f >= (U2048_TILE_DELTA - 1))
                    {
                        for(int xSwap = x; xSwap >= 0; xSwap--)
                        {
                            U2048Tile_t swap = game->Tiles[xSwap - 1][y];
                            game->Tiles[xSwap][y] = swap;
                        }

                        game->Tiles[0][y] = U2048Tile_Undefined;
                        U2048NewTile(game, 0, y, U2048Tile_Empty);
                    }
                }
                
                x++;
            }
        }
    }
}

void U2048MergeRight(U2048_t *game)
{
    for(int y = 0; y < U2048_GAME_SIZE; y++)
    {
        for(int x = U2048_TILE_COUNT; x > 0; x--)
        {
            if(game->Tiles[x][y] != U2048Tile_Empty
                && game->Tiles[x][y] == game->Tiles[x - 1][y])
            {
                for(int f = 0; f < U2048_TILE_DELTA; f += U2048_TILE_MOVE_STEP)
                {
                    U2048RenderStart(game);
                    U2048RenderBoard(game);
                    
                    for(int i = U2048_TILE_COUNT; i >= 0; i--)
                    {
                        for(int j = 0; j < U2048_GAME_SIZE; j++)
                        {
                            FT800Point_t p1;
                            FT800Point_t p2;
                            
                            U2048GetTileCorners(&p1, &p2, i, j);
                            
                            if(j == y && i < x)
                            {
                                p1.X += f;
                                p2.X += f;
                            }
                            
                            U2048RenderTile(game, p1, p2, game->Tiles[i][j]);
                        }
                    }
                    
                    U2048RenderFinish(game);
                    
                    if(f >= (U2048_TILE_DELTA - 1))
                    {
                        for(int xSwap = x; xSwap >= 0; xSwap--)
                        {
                            U2048Tile_t swap = game->Tiles[xSwap - 1][y];
                            game->Tiles[xSwap][y] = swap;
                        }

                        game->Tiles[0][y] = U2048Tile_Undefined;
                        U2048NewTile(game, 0, y, U2048Tile_Empty);
                        
                        U2048Tile_t newTile = U2048NextTile(game->Tiles[x][y]);
                        U2048NewTile(game, x, y, newTile);
                    }
                }
            }
        }
    }
}

void U2048JustifyLeft(U2048_t *game)
{
    for(int y = 0; y < U2048_GAME_SIZE; y++)
    {
        for(int x = 0; x < U2048_GAME_SIZE; x++)
        {
            if(game->Tiles[x][y] == U2048Tile_Empty)
            {
                bool emptyToEnd = true;
                for(int i = x + 1; i < U2048_GAME_SIZE; i++)
                {
                    if(game->Tiles[i][y] != U2048Tile_Empty)
                    {
                        emptyToEnd = false;
                        break;
                    }
                }

                if(emptyToEnd)
                    break;
                
                for(int f = 0; f < U2048_TILE_DELTA; f += U2048_TILE_MOVE_STEP)
                {
                    U2048RenderStart(game);
                    U2048RenderBoard(game);
                    
                    for(int i = 0; i < U2048_GAME_SIZE; i++)
                    {
                        for(int j = 0; j < U2048_GAME_SIZE; j++)
                        {
                            FT800Point_t p1;
                            FT800Point_t p2;
                            
                            U2048GetTileCorners(&p1, &p2, i, j);
                            
                            if(j == y && i > x)
                            {
                                p1.X -= f;
                                p2.X -= f;
                            }
                            
                            U2048RenderTile(game, p1, p2, game->Tiles[i][j]);
                        }
                    }
                    
                    U2048RenderFinish(game);

                    if(f >= (U2048_TILE_DELTA - 1))
                    {
                        for(int xSwap = x; xSwap < U2048_GAME_SIZE; xSwap++)
                        {
                            U2048Tile_t swap = game->Tiles[xSwap + 1][y];
                            game->Tiles[xSwap][y] = swap;
                        }
                        
                       game->Tiles[U2048_TILE_COUNT][y] = U2048Tile_Undefined;
                       U2048NewTile(game, U2048_TILE_COUNT, y, U2048Tile_Empty);
                    }
                }

                x--;
            }
        }
    }
}

void U2048MergeLeft(U2048_t *game)
{
    for(int y = 0; y < U2048_GAME_SIZE; y++)
    {
        for(int x = 0; x < U2048_GAME_SIZE; x++)
        {
            if(game->Tiles[x][y] != U2048Tile_Empty
                && game->Tiles[x][y] == game->Tiles[x + 1][y])
            {
                for(int f = 0; f < U2048_TILE_DELTA; f += U2048_TILE_MOVE_STEP)
                {
                    U2048RenderStart(game);
                    U2048RenderBoard(game);
                    
                    for(int i = U2048_TILE_COUNT; i >= 0; i--)
                    {
                        for(int j = 0; j < U2048_GAME_SIZE; j++)
                        {
                            FT800Point_t p1;
                            FT800Point_t p2;
                            
                            U2048GetTileCorners(&p1, &p2, i, j);
                            
                            if(j == y && i > x)
                            {
                                p1.X -= f;
                                p2.X -= f;
                            }
                            
                            U2048RenderTile(game, p1, p2, game->Tiles[i][j]);
                        }
                    }
                    
                    U2048RenderFinish(game);
                    
                    if(f >= (U2048_TILE_DELTA - 1))
                    {
                        for(int xSwap = x; xSwap < U2048_GAME_SIZE; xSwap++)
                        {
                            U2048Tile_t swap = game->Tiles[xSwap + 1][y];
                            game->Tiles[xSwap][y] = swap;
                        }

                       game->Tiles[U2048_TILE_COUNT][y] = U2048Tile_Undefined;
                       U2048NewTile(game, U2048_TILE_COUNT, y, U2048Tile_Empty);
                        
                       U2048Tile_t newTile = U2048NextTile(game->Tiles[x][y]);
                       U2048NewTile(game, x, y, newTile);
                    }
                }
            }
        }
    }
}

void U2048JustifyUp(U2048_t *game)
{
    for(int x = 0; x < U2048_GAME_SIZE; x++)
    {
        for(int y = 0; y < U2048_GAME_SIZE; y++)
        {
            if(game->Tiles[x][y] != U2048Tile_Empty)
                continue;

            bool emptyToEnd = true;
            for(int i = y + 1; i < U2048_GAME_SIZE; i++)
            {
                if(game->Tiles[x][i] != U2048Tile_Empty)
                {
                    emptyToEnd = false;
                    break;
                }
            }

            if(emptyToEnd)
                break;
            
            for(int f = 0; f < U2048_TILE_DELTA; f += U2048_TILE_MOVE_STEP)
            {
                U2048RenderStart(game);
                U2048RenderBoard(game);
                
                for(int i = 0; i < U2048_GAME_SIZE; i++)
                {
                    for(int j = 0; j < U2048_GAME_SIZE; j++)
                    {
                        FT800Point_t p1;
                        FT800Point_t p2;
                        
                        U2048GetTileCorners(&p1, &p2, i, j);
                        
                        if(i == x && j > y)
                        {
                            p1.Y -= f;
                            p2.Y -= f;
                        }
                        
                        U2048RenderTile(game, p1, p2, game->Tiles[i][j]);
                    }
                }
                
                U2048RenderFinish(game);

                if(f >= (U2048_TILE_DELTA - 1))
                {
                    for(int ySwap = y; ySwap < U2048_GAME_SIZE; ySwap++)
                    {
                        U2048Tile_t swap = game->Tiles[x][ySwap + 1];
                        game->Tiles[x][ySwap] = swap;
                    }
                    
                    game->Tiles[x][U2048_TILE_COUNT] = U2048Tile_Undefined;
                    U2048NewTile(game, x, U2048_TILE_COUNT, U2048Tile_Empty);
                }
            }
            
            y--;
        }
    }
}

void U2048MergeUp(U2048_t *game)
{
    for(int x = 0; x < U2048_GAME_SIZE; x++)
    {
        for(int y = 0; y < U2048_GAME_SIZE; y++)
        {
            if(game->Tiles[x][y] != U2048Tile_Empty
                && game->Tiles[x][y] == game->Tiles[x][y + 1])
            {
                for(int f = 0; f < U2048_TILE_DELTA; f += U2048_TILE_MOVE_STEP)
                {
                    U2048RenderStart(game);
                    U2048RenderBoard(game);
                    
                    for(int i = 0; i < U2048_GAME_SIZE; i++)
                    {
                        for(int j = 0; j < U2048_GAME_SIZE; j++)
                        {
                            FT800Point_t p1;
                            FT800Point_t p2;
                            
                            U2048GetTileCorners(&p1, &p2, i, j);
                            
                            if(i == x && j > y)
                            {
                                p1.Y -= f;
                                p2.Y -= f;
                            }
                            
                            U2048RenderTile(game, p1, p2, game->Tiles[i][j]);
                        }
                    }
                    
                    U2048RenderFinish(game);
                    
                    if(f >= (U2048_TILE_DELTA - 1))
                    {
                        for(int ySwap = y; ySwap < U2048_GAME_SIZE; ySwap++)
                        {
                            U2048Tile_t swap = game->Tiles[x][ySwap + 1];
                            game->Tiles[x][ySwap] = swap;
                        }

                       game->Tiles[x][U2048_TILE_COUNT] = U2048Tile_Undefined;
                       U2048NewTile(game, x, U2048_TILE_COUNT, U2048Tile_Empty);
                        
                       U2048Tile_t newTile = U2048NextTile(game->Tiles[x][y]);
                       U2048NewTile(game, x, y, newTile);
                    }
                }
            }
        }
    }
}

void U2048JustifyDown(U2048_t *game)
{
    for(int x = 0; x < U2048_GAME_SIZE; x++)
    {
        for(int y = U2048_TILE_COUNT; y > 0; y--)
        {
            if(game->Tiles[x][y] != U2048Tile_Empty)
                continue;
            
            bool emptyToEnd = true;
            for(int i = 0; i < y; i++)
            {
                if(game->Tiles[x][i] != U2048Tile_Empty)
                {
                    emptyToEnd = false;
                    break;
                }
            }

            if(emptyToEnd)
                break;
            
            for(int f = 0; f < U2048_TILE_DELTA; f += U2048_TILE_MOVE_STEP)
            {
                U2048RenderStart(game);
                U2048RenderBoard(game);
                
                for(int i = 0; i < U2048_GAME_SIZE; i++)
                {
                    for(int j = U2048_TILE_COUNT; j >= 0; j--)
                    {
                        FT800Point_t p1;
                        FT800Point_t p2;
                        
                        U2048GetTileCorners(&p1, &p2, i, j);
                        
                        if(i == x && j < y)
                        {
                            p1.Y += f;
                            p2.Y += f;
                        }
                        
                        U2048RenderTile(game, p1, p2, game->Tiles[i][j]);
                    }
                }
                
                U2048RenderFinish(game);
                
                if(f >= (U2048_TILE_DELTA - 1))
                {
                    for(int ySwap = y; ySwap >= 0; ySwap--)
                    {
                        U2048Tile_t swap = game->Tiles[x][ySwap - 1];
                        game->Tiles[x][ySwap] = swap;
                    }

                    game->Tiles[x][0] = U2048Tile_Undefined;
                    U2048NewTile(game, x, 0, U2048Tile_Empty);
                }
            }
            
            y++;
        }
    }
}

void U2048MergeDown(U2048_t *game)
{
    for(int x = 0; x < U2048_GAME_SIZE; x++)
    {
        for(int y = U2048_TILE_COUNT; y > 0; y--)
        {
            if(game->Tiles[x][y] != U2048Tile_Empty
                && game->Tiles[x][y] == game->Tiles[x][y - 1])
            {
                for(int f = 0; f < U2048_TILE_DELTA; f += U2048_TILE_MOVE_STEP)
                {
                    U2048RenderStart(game);
                    U2048RenderBoard(game);
                    
                    for(int i = 0; i < U2048_GAME_SIZE; i++)
                    {
                        for(int j = 0; j < U2048_GAME_SIZE; j++)
                        {
                            FT800Point_t p1;
                            FT800Point_t p2;
                            
                            U2048GetTileCorners(&p1, &p2, i, j);
                            
                            if(i == x && j < y)
                            {
                                p1.Y += f;
                                p2.Y += f;
                            }
                            
                            U2048RenderTile(game, p1, p2, game->Tiles[i][j]);
                        }
                    }
                    
                    U2048RenderFinish(game);
                    
                    if(f >= (U2048_TILE_DELTA - 1))
                    {
                        for(int ySwap = y; ySwap >= 0; ySwap--)
                        {
                            U2048Tile_t swap = game->Tiles[x][ySwap - 1];
                            game->Tiles[x][ySwap] = swap;
                        }

                       game->Tiles[x][0] = U2048Tile_Undefined;
                       U2048NewTile(game, x, 0, U2048Tile_Empty);
                        
                       U2048Tile_t newTile = U2048NextTile(game->Tiles[x][y]);
                       U2048NewTile(game, x, y, newTile);
                    }
                }
            }
        }
    }
}

void U2048Action(U2048_t *game, U2048Action_t action)
{
    switch(action)
    {
        case U2048Action_SwipeRight:
            U2048JustifyRight(game);
            U2048MergeRight(game);
            U2048JustifyRight(game);
            break;
        case U2048Action_SwipeLeft:
            U2048JustifyLeft(game);
            U2048MergeLeft(game);
            U2048JustifyLeft(game);
            break;
        case U2048Action_SwipeUp:
            U2048JustifyUp(game);
            U2048MergeUp(game);
            U2048JustifyUp(game);
            break;
        case U2048Action_SwipeDown:
            U2048JustifyDown(game);
            U2048MergeDown(game);
            U2048JustifyDown(game);
            break;
    }
    
    // Randomly place a tile, mostly 2 sometimes 4 if tiles were moved
}

U2048Tile_t U2048NextTile(U2048Tile_t tile)
{
    if(tile == U2048Tile_Empty)
    {
        return U2048Tile_2;
    }
    else if(tile < U2048Tile_2048)
    {
        return tile * 2;
    }
    else
    {
        return U2048Tile_2048;
    }
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
    for(int i = 0; i < U2048_GAME_SIZE; i++)
    {
        for(int j = 0; j < U2048_GAME_SIZE; j++)
        {
            if(game->Tiles[i][j] != U2048Tile_Undefined)
            {
                FT800Point_t p1;
                FT800Point_t p2;
                U2048GetTileCorners(&p1, &p2, i, j);
                
                U2048RenderTile(game, p1, p2, game->Tiles[i][j]);
            }
        }
    }
}

void U2048RenderTile(U2048_t *game,
    FT800Point_t p1, FT800Point_t p2, U2048Tile_t tile)
{
    FT800Color_t tileColor = U2048GetTileColor(tile);
    FT800DlRgb(game->ft800, tileColor);
    FT800DrawRectangle(game->ft800, p1, p2);
    U2048RenderTileString(game, tile, p1);
}

void U2048GetTileCorners(FT800Point_t *p1, FT800Point_t *p2, int x, int y)
{
    p1->X = ((x + 1) * U2048_TILE_SPACING) + (x * U2048_TILE_SIZE);
    p1->Y = ((y + 1) * U2048_TILE_SPACING) + (y * U2048_TILE_SIZE);

    p2->X = p1->X + U2048_TILE_SIZE;
    p2->Y = p1->Y + U2048_TILE_SIZE;
}

void U2048RenderTileString(U2048_t *game, U2048Tile_t tile, FT800Point_t p)
{
    if(tile != U2048Tile_Empty)
    {
        char str[5];
        sprintf(str, "%d", tile);
        
        p.X += U2048_TILE_SIZE / 2;
        p.Y += U2048_TILE_SIZE / 2;
        
        FT800Color_t textColor = U2048GetTextColor(tile);
        FT800DlRgb(game->ft800, textColor);
        
        FT800CmdDrawText(game->ft800, p, FT800Font_AntiAliased3,
                FT800Option_CenterX | FT800Option_CenterY,
                str, strlen(str));
    }
}

void U2048RenderFinish(U2048_t *game)
{
    FT800DlEnd(game->ft800);
    FT800CmdSwapDisplayList(game->ft800);
    FT800CmdFlush(game->ft800);
}

