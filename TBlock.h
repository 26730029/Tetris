#ifndef TBLOCK_H
#define TBLOCK_H

#include "Blocks.h"

class TBlock : public Blocks
{
public:
    TBlock()
    {
        char initialShape[4][4] = {
            {' ', ' ', ' ', ' '},
            {' ', 'T', ' ', ' '},
            {'T', 'T', 'T', ' '},
            {' ', ' ', ' ', ' '}
        };

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                shape[i][j] = initialShape[i][j];
            }
        }
    }
};

#endif