/**
 * Author: Brad Gale
 *
 * This file contains the Cell class and related methods
 */

#include <iostream>
#include <string>
#include "cell.h"
#include "ANSI-color-codes.h"

using namespace std;

Cell::Cell() : m_status{0},
               m_revealed{false},
               m_mine{false},
               m_flagged{false} {};

ostream &operator<<(ostream &os, const Cell &cell)
{
    switch (cell.m_status)
    {
    case 0:
        return os << " ";
        break;

    case 1:
        return os << BLU << "1" << reset;

    case 2:
        return os << GRN << "2" << reset;

    case 3:
        return os << RED << "3" << reset;

    case 4:
        return os << MAG << "4" << reset;

    case 5:
        return os << YEL << "5" << reset;
    
    case 6:
        return os << CYN << "6" << reset;
    
    case 7:
        return os << BBLU << "7" << reset;

    case 8:
        return os << BWHT << "8" << reset;
    
    default:
        return os << " ";
    }
}