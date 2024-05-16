/**
 * Author: Brad Gale
 *
 * Cell definitions
 */

#pragma once

using namespace std;

class Cell
{
    private:
    int m_status;
    bool m_revealed;
    bool m_mine;
    bool m_flagged;

    public:
    // Constructor
    // Each cell is initially set to a status of 0, no mine, and no flags.
    Cell();    
    
    // Getters
    int getStatus() const { return m_status; };
    bool isRevealed() const { return m_revealed; };
    bool isMine() const { return m_mine; };
    bool isFlagged() const { return m_flagged; };

    // Setter
    void setStatus(int status) { m_status = status; };
    void setRevealed(bool revealed) { m_revealed = revealed; };
    void setMine(bool isMine) { m_mine = isMine; };
    void setFlagged(bool isFlagged) { m_flagged = isFlagged; };

    // << Overload for printing the cells to the board
    friend ostream &operator<<(ostream &os, const Cell &cell);
};

