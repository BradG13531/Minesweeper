/**
 * Author: Brad Gale
 * 
 * Functions that handle player input. These funcitons are separate from 
 * main to keep the main file easier to read.
*/
#pragma once
#include "minefield.h"

// Prompts user for input to set up the baord and creates the baord 
// itself.
Minefield* boardSetup();

// Prompts user for first move and calls createMines() and initializeBoard()
void firstMove(Minefield& minefield);
// 
void playerInput(Minefield& minefield);