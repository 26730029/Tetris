#ifndef OBLOCK_H
#define OBLOCK_H

#include "Blocks.h"

class OBlock : public Blocks
{
public:
    OBlock()
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = ' ';

        shape[1][1] = 'O';
        shape[1][2] = 'O';
        shape[2][1] = 'O';
        shape[2][2] = 'O';
    }

    void rotate() override {}
};

#endif
