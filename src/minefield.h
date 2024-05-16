/**
 * Author: Brad Gale
 *
 * Minefield definitions
 */

#pragma once
#include <iostream>
#include <vector>
#include "cell.h"

using namespace std;

class Minefield
{
private:
    int m_rows;
    int m_cols;
    int m_mines;
    int m_flags;
    int m_unrevealed; // The number of Cells that are not revealed
    bool m_debug; // Whether the board should also print debug() or not

public:
    
    // The minefield itself
    vector< vector<Cell> > minefield;

    // Constructor
    Minefield(int rows, int cols, int mines);

    // Creates the mines and places them in random locations throughout the board.
    void createMines(int startRow, int startCol);

    // Fills in the rest of the spaces with cells and their status.
    void initializeBoard();

    // Reveals the area around the inputted starting cell until a mine is found.
    // Incorporates bredth-first search with a queue
    void startingArea(int row, int col);

    // If the user selects a cell with a 0, this method will keep revealing zeroes
    // and surrounding cells until no more zeroes appear.
    // Incorporates depth-first search using a stack
    void revealZeroes(int row, int col);

    /**
     * Takes in the user guess and verifies that the move is legal before setting
     * the appropriate cell to be revealed. Will return true if the move is legal, 
     * false is the move is not. A non legal move is a guess that is outside of the 
     * board. Also calls gameOver() if the guess reveals a mine.
    */
    bool guess(int row, int col, bool flag);

    /**
     * Checks conditions of a game over
     * User Won:
     * -Every non mine space is revealed
     *
     * User Lost:
     * -The user has revealed a mine
     */
    bool gameOver();

    // Converts a number to a letter, used for the printing functions below
    char numToLetter(int n);

    // Prints empty board for before the game starts
    void printEmpty();

    /**
     * Prints out the board with every cell revealed. 
     * Does not show flags.
    */
    void debug();

    /**
     * Prints out the board with all rules in place
     * Flags are shown, non-revealed Cells are not shown.
    */
    void print();

    // Displays information about the minefield
    friend ostream &operator<<(ostream &os, const Minefield &minefield);

    // Getters
    int getRows() const { return m_rows; };
    int getCols() const { return m_cols; };
    int getMines() const { return m_mines; };
    int getFlags() const { return m_flags; };
    int getNotRevealed() const { return m_unrevealed; };

    // Setters
    void placedFlag() { m_flags--; };
    void removedFlag() { m_flags++; };
    void revealedCell() { m_unrevealed--; };
    void setDebug() { m_debug = true; };
};