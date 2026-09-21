#ifndef ZBLOCK_H
#define ZBLOCK_H

#include "Blocks.h"

class ZBlock : public Blocks {
public:
    ZBlock() {
        char initialShape[4][4] = {
            {' ', ' ', ' ', ' '},
            {'Z', 'Z', ' ', ' '},
            {' ', 'Z', 'Z', ' '},
            {' ', ' ', ' ', ' '}
        };

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                shape[i][j] = initialShape[i][j];
            }
        }
    }
};

#endif
