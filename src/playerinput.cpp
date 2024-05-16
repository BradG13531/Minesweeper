/**
 * Author: Brad Gale
 *
 * Definitions for player input related functions
 */
#include <iostream>
#include <string>
#include <limits>
#include "playerinput.h"
#include "minefield.h"

using namespace std;

Minefield *boardSetup()
{
    cout << "Minesweeper" << '\n';
    cout << '\n';
    cout << "Play in debug mode? (y/n): ";
    bool validInput{false};
    bool debug{false};
    while (!validInput)
    {
        char input;
        cin >> input;
        if (!cin)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        else
        {
            if (input == 'y')
            {
                debug = true;
                validInput = true;
            }
            else if (input == 'n')
            {
                debug = false;
                validInput = true;
            }
            else
            {
                cout << "Input not valid. Debug:" << '\n';
            }
        }
    }

    cout << '\n'
         << "Choose a difficulty:" << '\n';
    cout << "Easy: 9x9 with 10 mines" << '\n';
    cout << "Medium: 16x16 with 40 mines" << '\n';
    cout << "Hard: 20x20 with 80 mines" << '\n';
    cout << "Custom: Choose your own values" << '\n';

    while (true)
    {
        string decision{""};
        cin >> decision;
        decision.erase(remove(decision.begin(), decision.end(), ' '), decision.end());
        for (int i{0}; i < decision.length(); i++)
        {
            decision[i] = tolower(decision[i]);
        }

        if (decision == "easy")
        {
            Minefield *minefield{new Minefield(9, 9, 10)};
            if (debug)
            {
                minefield->setDebug();
            }
            return minefield;
        }
        else if (decision == "medium")
        {
            Minefield *minefield{new Minefield(16, 16, 40)};
            if (debug)
            {
                minefield->setDebug();
            }
            return minefield;
        }
        else if (decision == "hard")
        {
            Minefield *minefield{new Minefield(20, 20, 80)};
            if (debug)
            {
                minefield->setDebug();
            }
            return minefield;
        }
        else if (decision == "custom")
        {
            int rows;
            int cols;
            int mines;
            bool validInput{false};
            while (!validInput)
            {
                cout << "Enter number of rows (26 or less): " << '\n';
                cin >> rows;
                if (!cin)
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                if (rows > 0 && rows <= 26)
                {
                    validInput = true;
                }
                else if (rows > 26)
                {
                    cout << "Please enter a number that is 26 or less." << '\n';
                }
                else if (rows <= 0)
                {
                    cout << "Please enter a positive number." << '\n';
                }
                else
                {
                    cout << "Please enter a valid number." << '\n';
                }
            }
            validInput = false;
            while (!validInput)
            {
                cout << "Enter number of columns (26 or less): " << '\n';
                cin >> cols;
                if (!cin)
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                if (cols > 0 && cols <= 26)
                {
                    validInput = true;
                }
                else if (cols > 26)
                {
                    cout << "Please enter a number that is 26 or less." << '\n';
                }
                else if (cols <= 0)
                {
                    cout << "Please enter a positive number." << '\n';
                }
                else
                {
                    cout << "Please enter a valid number." << '\n';
                }
            }
            int maxMines = rows * cols * .3;
            validInput = false;
            while (!validInput)
            {
                cout << "Enter number of mines (" << maxMines << " or less): " << '\n';
                cin >> mines;
                if (!cin)
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                if (mines > 0 && mines <= maxMines)
                {
                    validInput = true;
                }
                else if (mines > maxMines)
                {
                    cout << "Too many mines." << '\n';
                }
                else if (mines <= 0)
                {
                    cout << "Please enter a positive number." << '\n';
                }
                else
                {
                    cout << "Please enter a valid number." << '\n';
                }
            }
            Minefield *minefield = new Minefield(rows, cols, mines);
            if (debug)
            {
                minefield->setDebug();
            }
            return minefield;
        }
        else
        {
            cout << "Please enter a valid option" << '\n';
        }
    }
};

void firstMove(Minefield &minefield)
{
    minefield.printEmpty();
    int numRows = minefield.getRows();
    int numCols = minefield.getCols();

    cout << "Enter starting position (Format: {Column Row}): " << '\n';
    bool validInput{false};
    while (!validInput)
    {
        string input{""};
        cin >> input;
        input.erase(remove(input.begin(), input.end(), ' '), input.end());
        input.erase(remove(input.begin(), input.end(), ','), input.end());
        input.erase(remove(input.begin(), input.end(), '{'), input.end());
        input.erase(remove(input.begin(), input.end(), '}'), input.end());
        for (int i{0}; i < input.length(); i++)
        {
            input[i] = tolower(input[i]);
        }
        if (!cin)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        else if (input.length() < 2)
        {
            cout << "Please enter a valid cell: " << '\n';
        }
        else
        {
            if (isalpha(input[0]))
            {
                int col = input[0] - 'a';
                if (col < numCols)
                {
                    if (isdigit(input[1]) && (numRows < 10))
                    {
                        if (input.length() >= 3 && isdigit(input[2])) // User entered number with two digits
                        {
                            cout << "Please choose cell inside of grid: " << '\n';
                        }
                        else
                        {
                            int row = input[1] - '0';
                            if (row < numRows)
                            {
                                minefield.createMines(row, col);
                                minefield.initializeBoard();
                                minefield.startingArea(row, col);
                                validInput = true;
                            }
                            else
                            {
                                cout << "Please choose cell inside of grid: " << '\n';
                            }
                        }
                    }
                    else if ((input.length() == 2) && isdigit(input[1]))
                    {
                        int row = input[1] - '0';
                        if (row < numRows)
                        {
                            minefield.createMines(row, col);
                            minefield.initializeBoard();
                            minefield.startingArea(row, col);
                            validInput = true;
                        }
                        else
                        {
                            cout << "Please choose cell inside of grid: " << '\n';
                        }
                    }
                    else if ((numRows >= 10) && (input.length() == 3) && isdigit(input[1]) && isdigit(input[2]))
                    {
                        string tempRow;
                        tempRow[0] = input[1];
                        tempRow[1] = input[2];
                        int row = stoi(tempRow);
                        if (row < numRows)
                        {
                            minefield.createMines(row, col);
                            minefield.initializeBoard();
                            minefield.startingArea(row, col);
                            validInput = true;
                        }
                        else
                        {
                            cout << "Please choose cell inside of grid: " << '\n';
                        }
                    }
                    else
                    {
                        cout << "Please use correct format: " << '\n';
                    }
                }
                else
                {
                    cout << "Please choose cell inside of grid: " << '\n';
                }
            }
            else
            {
                cout << "Please use correct format: " << '\n';
            }
        }
    }
};

void playerInput(Minefield &minefield)
{
    minefield.print();
    int numRows = minefield.getRows();
    int numCols = minefield.getCols();

    cout << "Remaining Flags: " << minefield.getFlags() << '\n';
    cout << "Enter move and flag (Format: {Column Row (Flag: f)}): " << '\n';
    bool validInput{false};
    while (!validInput)
    {
        string input{""};
        cin >> input;
        input.erase(remove(input.begin(), input.end(), ' '), input.end());
        input.erase(remove(input.begin(), input.end(), ','), input.end());
        input.erase(remove(input.begin(), input.end(), '{'), input.end());
        input.erase(remove(input.begin(), input.end(), '}'), input.end());
        for (int i{0}; i < input.length(); i++)
        {
            input[i] = tolower(input[i]);
        }
        if (!cin)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        else if (input.length() < 2)
        {
            cout << "Please enter a valid cell: " << '\n';
        }
        else
        {
            if (isalpha(input[0]))
            {
                int col = input[0] - 'a';
                if (col < numCols)
                {
                    if (input.length() == 2) // Row and Column no flag
                    {
                        if (isdigit(input[1]))
                        {
                            int row = input[1] - '0';
                            if (row < numRows)
                            {
                                validInput = minefield.guess(row, col, false);
                            }
                            else
                            {
                                cout << "Please enter cell inside of grid: " << '\n';
                            }
                        }
                        else
                        {
                            cout << "Please use correct format: " << '\n';
                        }
                    }
                    else if (input.length() == 3) // Row and Column with flag or without flag
                    {
                        if (isdigit(input[1]) && isdigit(input[2])) // No flag
                        {
                            string tempRow;
                            tempRow[0] = input[1];
                            tempRow[1] = input[2];
                            int row = stoi(tempRow);
                            if (row < numRows)
                            {
                                validInput = minefield.guess(row, col, false);
                            }
                            else
                            {
                                cout << "Please enter cell inside of grid: " << '\n';
                            }
                        }
                        else if (isdigit(input[1]) && isalpha(input[2])) // Flag
                        {
                            int row = input[1] - '0';
                            if (row < numRows)
                            {
                                if (input[2] == 'f' || input[2] == 'F')
                                {
                                    validInput = minefield.guess(row, col, true);
                                }
                                else
                                {
                                    cout << "Use F or f to place a flag: " << '\n';
                                }
                            }
                            else
                            {
                                cout << "Please enter cell inside of grid: " << '\n';
                            }
                        }
                        else
                        {
                            cout << "Please use correct format: " << '\n';
                        }
                    }
                    else if (input.length() == 4) // Row and Column with flag
                    {
                        if (isdigit(input[1]) && isdigit(input[2]) && isalpha(input[3]))
                        {
                            string tempRow;
                            tempRow[0] = input[1];
                            tempRow[1] = input[2];
                            int row = stoi(tempRow);
                            if (row < numRows)
                            {
                                if (input[3] == 'f' || input[3] == 'F')
                                {
                                    validInput = minefield.guess(row, col, true);
                                }
                                else
                                {
                                    cout << "Use F or f to place a flag: " << '\n';
                                }
                            }
                            else
                            {
                                cout << "Please enter cell inside of grid: " << '\n';
                            }
                        }
                        else
                        {
                            cout << "Please use correct format: " << '\n';
                        }
                    }
                    else
                    {
                        cout << "Please enter cell inside of grid: " << '\n';
                    }
                }
                else
                {
                    cout << "Please choose cell inside of grid: " << '\n';
                }
            }
            else
            {
                cout << "Please use correct format: " << '\n';
            }
        }
    }
};