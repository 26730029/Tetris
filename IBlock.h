#ifndef IBLOCK_H
#define IBLOCK_H

#include "Blocks.h"

class IBlock : public Blocks
{
public:
    IBlock()
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = ' ';

        shape[0][2] = 'I';
        shape[1][2] = 'I';
        shape[2][2] = 'I';
        shape[3][2] = 'I';
    }
};

#endif
