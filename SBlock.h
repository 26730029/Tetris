#ifndef SBLOCK_H
#define SBLOCK_H

#include "Blocks.h"

class SBlock : public Blocks
{
public:
    SBlock()
    {
        char initialShape[4][4] = {
            {' ', ' ', ' ', ' '},
            {' ', 'S', 'S', ' '},
            {'S', 'S', ' ', ' '},
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