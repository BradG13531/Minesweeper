/**
 * Author: Brad Gale
 *
 * This file contains the class for the minefield and related methods.
 */

#include <iostream>
#include <vector>
#include <random>
#include <queue>
#include <stack>
#include "minefield.h"
#include "cell.h"
#include "ANSI-color-codes.h"

using namespace std;

Minefield::Minefield(int rows, int cols, int mines)
    : m_mines{mines},
      m_flags{mines},
      m_debug{false}
{
    if (rows > 26)
    {
        m_rows = 26;
    }
    else
    {
        m_rows = rows;
    }
    if (cols > 26)
    {
        m_cols = 26;
    }
    else
    {
        m_cols = cols;
    }

    m_unrevealed = m_rows * m_cols;
    cout << "Original unrevealed: " << m_unrevealed << '\n';

    minefield.resize(m_rows, vector<Cell>(m_cols));
}

/**
 * Uses Mersenne Twister to generate random numbers
 */
void Minefield::createMines(int startRow, int startCol)
{
    int numMines = m_mines;

    // Instantiate a 32-bit Mersenne Twister that uses
    // a random_device as a seed.
    mt19937 mt{random_device{}()};

    // Uniform distribution to stay inside the bounds of the board
    uniform_int_distribution randRow{0, getRows() - 1};
    uniform_int_distribution randCol{0, getCols() - 1};

    while (numMines > 0)
    {
        int thisRow{randRow(mt)};
        int thisCol{randCol(mt)};

        if (!minefield[thisRow][thisCol].isMine() && !(thisRow == startRow && thisCol == startCol))
        {
            minefield[thisRow][thisCol].setMine(true);
            numMines--;
        }
    }

    // Used for debugging purposes, displays all mines.
    // for (int i = 0; i < this->getRows(); i++) {
    //     for (int j = 0; j < this->getCols(); j++) {
    //         cout << minefield[i][j].isMine() << '\t';
    //     }

    //     cout << '\n';
    // }
};

/**
 * Counts the number of mines surrounding each Cell and sets
 * the status accordingly
 */
void Minefield::initializeBoard()
{
    // Outter two loops go through each individual Cell
    for (int row = 0; row < m_rows; row++)
    {
        for (int col = 0; col < m_cols; col++)
        {
            // Iterates through the adjacent 8 cells to find and set the number of surrounding mines
            if (!minefield[row][col].isMine())
            {
                int minesFound{0};
                for (int i = -1; i < 2; i++)
                {
                    for (int j = -1; j < 2; j++)
                    {
                        if ((row + i >= 0) && (row + i < m_rows) && (col + j >= 0) && (col + j < m_cols) &&
                            !((row == i) && (col == j)) && minefield[row + i][col + j].isMine())
                        {
                            minesFound++;
                        }
                    }
                }
                minefield[row][col].setStatus(minesFound);
            } // Inner Loop
        }
    } // Outter loop
};

void Minefield::startingArea(int row, int col)
{
    // Initialize the queue to store locations
    vector<int> origLocation{row, col};
    queue<vector<int>> queue;
    queue.push(origLocation);

    while (!queue.empty())
    {
        // Remove the front of the queue
        vector<int> location = queue.front();
        queue.pop();

        // If the current location is a mine, the search is done
        if (minefield[location[0]][location[1]].isMine())
        {
            break;
        }

        // Otherwise the four adjacent cells are added to the queue if they haven't
        // already been visited
        else
        {
            // If 0 is found, keep revealing surrounding 0's
            if (minefield[location[0]][location[1]].getStatus() == 0)
            {
                revealZeroes(location[0], location[1]);
            }
            else if (!(minefield[location[0]][location[1]].isRevealed()))
            {
                minefield[location[0]][location[1]].setRevealed(true);
                revealedCell();
            }

            if (location[0] - 1 >= 0)
            {
                if (!(minefield[location[0] - 1][location[1]].isRevealed()))
                {
                    vector<int> nextLocation{location[0] - 1, location[1]};
                    queue.push(nextLocation);
                }
            }
            if (location[0] + 1 < m_rows)
            {
                if (!(minefield[location[0] + 1][location[1]].isRevealed()))
                {
                    vector<int> nextLocation{location[0] + 1, location[1]};
                    queue.push(nextLocation);
                }
            }
            if (location[1] - 1 >= 0)
            {
                if (!(minefield[location[0]][location[1] - 1].isRevealed()))
                {
                    vector<int> nextLocation{location[0], location[1] - 1};
                    queue.push(nextLocation);
                }
            }
            if (location[1] + 1 < m_cols)
            {
                if (!(minefield[location[0]][location[1] + 1].isRevealed()))
                {
                    vector<int> nextLocation{location[0], location[1] + 1};
                    queue.push(nextLocation);
                }
            }
        }
    }
};

void Minefield::revealZeroes(int row, int col)
{
    // Initialize the stack with the input location
    vector<int> origLocation{row, col};
    stack<vector<int>> stack;
    stack.push(origLocation);

    while (!stack.empty())
    {
        vector<int> location = stack.top();
        stack.pop();

        if (!(minefield[location[0]][location[1]].isRevealed()))
        {
            minefield[location[0]][location[1]].setRevealed(true);
            revealedCell();
        }

        // Reveal cells surrounding the current 0 that aren't also 0's.
        // This gives a border of numbers around a pool of 0's so the user
        // doesn't need to manually reveal all of them.
        for (int i{-1}; i < 2; i++)
        {
            for (int j{-1}; j < 2; j++)
            {
                if ((location[0] + i >= 0) && (location[0] + i < m_rows) && (location[1] + j >= 0) && (location[1] + j < m_cols) &&
                    (minefield[location[0] + i][location[1] + j].getStatus() != 0) && !(minefield[location[0] + i][location[1] + j].isMine()) &&
                    !(minefield[location[0] + i][location[1] + j].isRevealed()) && !(i == 0 && j == 0))
                {
                    minefield[location[0] + i][location[1] + j].setRevealed(true);
                    revealedCell();
                }
            }
        }

        if ((location[0] - 1 >= 0) && !(minefield[location[0] - 1][location[1]].isRevealed()) &&
            (minefield[location[0] - 1][location[1]].getStatus() == 0) && !(minefield[location[0] - 1][location[1]].isMine()))
        {
            vector<int> nextLoc{location[0] - 1, location[1]};
            stack.push(nextLoc);
        }
        if ((location[0] + 1 < m_rows) && !(minefield[location[0] + 1][location[1]].isRevealed()) &&
            (minefield[location[0] + 1][location[1]].getStatus() == 0) && !(minefield[location[0] + 1][location[1]].isMine()))
        {
            vector<int> nextLoc{location[0] + 1, location[1]};
            stack.push(nextLoc);
        }
        if ((location[1] - 1 >= 0) && !(minefield[location[0]][location[1] - 1].isRevealed()) &&
            (minefield[location[0]][location[1] - 1].getStatus() == 0) && !(minefield[location[0]][location[1] - 1].isMine()))
        {
            vector<int> nextLoc{location[0], location[1] - 1};
            stack.push(nextLoc);
        }
        if ((location[1] + 1 < m_cols) && !(minefield[location[0]][location[1] + 1].isRevealed()) &&
            (minefield[location[0]][location[1] + 1].getStatus() == 0) && !(minefield[location[0]][location[1] + 1].isMine()))
        {
            vector<int> nextLoc{location[0], location[1] + 1};
            stack.push(nextLoc);
        }
    }
};

bool Minefield::guess(int row, int col, bool flag)
{
    if ((row < 0) || (row >= m_rows) || (col < 0) || (col >= m_cols))
    {
        return false;
    }
    else
    {
        // User places a flag on a non-revelaed cell
        if (flag && !minefield[row][col].isFlagged() && !minefield[row][col].isRevealed())
        {
            minefield[row][col].setFlagged(true);
            placedFlag();
            return true;
        }
        // User removes a flag
        else if (flag && minefield[row][col].isFlagged() && !(minefield[row][col].isRevealed()))
        {
            minefield[row][col].setFlagged(false);
            removedFlag();
            return true;
        }
        // User tries to reveal something with a flag on it
        else if (!flag && minefield[row][col].isFlagged())
        {
            return true;
        }
        // User tries to flag something that is revealed
        else if (flag && minefield[row][col].isRevealed())
        {
            return true;
        }
        // User reveals a cell
        else if (!flag && !(minefield[row][col].isRevealed()))
        {
            minefield[row][col].setRevealed(true);
            revealedCell();
            if (minefield[row][col].getStatus() == 0)
            {
                revealZeroes(row, col);
            }
            return true;
        }
        // User reveals an already revealed cell with flags placed correctly in adjacent cells,
        // so the non mine adjacent cells will be revealed.
        else if (!flag && minefield[row][col].isRevealed())
        {
            int flaggedMines{0};
            for (int i{-1}; i < 2; i++)
            {
                for (int j{-1}; j < 2; j++)
                {
                    if ((row + i >= 0) && (col + j >= 0) && (row + i < m_rows) && (col + j < m_cols) && minefield[i + row][j + col].isMine() &&
                        minefield[i + row][j + col].isFlagged())
                    {
                        flaggedMines++;
                    }
                }
            }
            if (flaggedMines == minefield[row][col].getStatus())
            {
                for (int i{-1}; i < 2; i++)
                {
                    for (int j{-1}; j < 2; j++)
                    {
                        if ((row + i >= 0) && (col + j >= 0) && (row + i < m_rows) && (col + j < m_cols) && !(minefield[i + row][j + col].isMine()) &&
                            !(minefield[i + row][j + col].isFlagged()) && !(minefield[row + i][col + j].isRevealed()))
                        {
                            if (minefield[i + row][j + col].getStatus() == 0)
                            {
                                revealZeroes(i + row, j + col);
                            }
                            else
                            {
                                minefield[i + row][j + col].setRevealed(true);
                                revealedCell();
                            }
                        }
                    }
                }
                return true;
            }
            else
            {
                return true;
            }
        }
        // Something went wrong
        else
        {
            cout << "Invalid Move" << '\n';
            return false;
        }
    }
}

bool Minefield::gameOver()
{
    int revealedMines{0};
    for (int i{0}; i < m_rows; i++)
    {
        for (int j{0}; j < m_cols; j++)
        {
            if (minefield[i][j].isMine() && minefield[i][j].isRevealed())
            {
                revealedMines++;
                break;
            }
        }
    }
    if (revealedMines > 0)
    {
        print();
        cout << "Game Over" << '\n';
        return true;
    }
    if (getNotRevealed() == getMines())
    {
        print();
        cout << "You Won!" << '\n';
        return true;
    }
    else
    {
        return false;
    }
};

char Minefield::numToLetter(int n)
{
    return "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[n];
}

void Minefield::printEmpty()
{
    cout << '\n';
    cout << "   ";

    // Print out top row letters and top line
    string letters = "";
    for (int i{0}; i < m_cols; i++)
    {
        letters += "  ";
        letters += numToLetter(i);
        letters += " ";
    }
    cout << letters << '\n'
         << "   +";
    for (int i{0}; i < m_cols; i++)
    {
        cout << "---+";
    }
    // Print out the rest
    cout << '\n';
    for (int row = 0; row < m_rows; row++)
    {
        if (row < 10)
        {
            cout << " " << row << " |";
        }
        else
        {
            cout << row << " |";
        }
        for (int col = 0; col < m_cols; col++)
        {
            cout << " " << BRED << " " << reset << " |";
        }
        cout << " " << row << '\n';
        cout << "   +";
        for (int i{0}; i < m_cols; i++)
        {
            cout << "---+";
        }
        cout << '\n';
    }
    cout << "   " << letters << '\n';
    cout << '\n';
}

void Minefield::debug()
{
    cout << '\n';
    cout << "   ";

    // Print out top row letters and top line
    string letters = "";
    for (int i{0}; i < m_cols; i++)
    {
        letters += "  ";
        letters += numToLetter(i);
        letters += " ";
    }
    cout << letters << '\n'
         << "   +";
    for (int i{0}; i < m_cols; i++)
    {
        cout << "---+";
    }
    // Print out the rest
    cout << '\n';
    for (int row = 0; row < m_rows; row++)
    {
        if (row < 10)
        {
            cout << " " << row << " |";
        }
        else
        {
            cout << row << " |";
        }
        for (int col = 0; col < m_cols; col++)
        {
            if (minefield[row][col].isMine())
            {
                cout << " " << BRED << "M" << reset << " |";
            }
            else
            {
                cout << " " << minefield[row][col] << " |";
            }
        }
        cout << " " << row << '\n';
        cout << "   +";
        for (int i{0}; i < m_cols; i++)
        {
            cout << "---+";
        }
        cout << '\n';
    }
    cout << "   " << letters << '\n';
    cout << '\n';
}

void Minefield::print()
{
    if (this->m_debug) // Print out debug board
    {
        this->debug();
    }

    cout << '\n';
    cout << "   ";
    // Print out top row letters and top line
    string letters = "";
    for (int i{0}; i < m_cols; i++)
    {
        letters += "  ";
        letters += numToLetter(i);
        letters += " ";
    }
    cout << letters << '\n'
         << "   +";
    for (int i{0}; i < m_cols; i++)
    {
        cout << "---+";
    }
    // Print out the rest
    cout << '\n';
    for (int row = 0; row < m_rows; row++)
    {
        if (row < 10)
        {
            cout << " " << row << " |";
        }
        else
        {
            cout << row << " |";
        }
        for (int col = 0; col < m_cols; col++)
        {
            if (minefield[row][col].isFlagged())
            {
                cout << " " << BYEL << "F" << reset << " |";
            }
            else if (minefield[row][col].isMine() && minefield[row][col].isRevealed())
            {
                cout << " " << BRED << "M" << reset << " |";
            }
            else if (minefield[row][col].isRevealed())
            {
                cout << " " << minefield[row][col] << " |";
            }
            else
            {
                cout << " - |";
            }
        }
        cout << " " << row << '\n';
        cout << "   +";
        for (int i{0}; i < m_cols; i++)
        {
            cout << "---+";
        }
        cout << '\n';
    }
    cout << "   " << letters << '\n';
    cout << '\n';
}

ostream &operator<<(ostream &os, const Minefield &minefield)
{
    return os << "Length: " << minefield.getCols() << " Height: " << minefield.getRows() << "\nMines: " << minefield.getMines() << " Flags: " << minefield.getFlags();
};