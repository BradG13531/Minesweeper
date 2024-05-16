/**
 * Author: Brad Gale
 *
 * This is the main function, the place where the game is started and the player
 * interaction occurs.
 */

#include <iostream>
#include "minefield.h"
#include "cell.h"
#include "playerinput.h"

using namespace std;

int main()
{
    // The board is created
    Minefield &minefield = *boardSetup();

    // The user is prompted with first move and board is instantiated
    firstMove(minefield);

    while (!minefield.gameOver()) // The game continues until gameover() is true
    {
        playerInput(minefield);
    }

    delete &minefield;

    return 0;
}