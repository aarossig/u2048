/*
 * Gameduino 2 on the STM32F4 Nucleo Board
 */

#ifndef GAMEDUINO2_H
#define GAMEDUINO2_H

#include "FT800.h"

/* Initialization *************************************************************/

bool Gameduino2Init(FT800_t *ft800);
void Gameduino2Blank(FT800_t *ft800);

#endif
