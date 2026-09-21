#ifndef LBLOCK_H
#define LBLOCK_H

#include "Blocks.h"

class LBlock : public Blocks {
public:
    LBlock() {
        char initialShape[4][4] = {
            {' ', ' ', ' ', ' '},
            {' ', ' ', 'L', ' '},
            {'L', 'L', 'L', ' '},
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