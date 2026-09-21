#ifndef JBLOCK_H
#define JBLOCK_H

#include "Blocks.h"

class JBlock : public Blocks {
public:
    JBlock() {
        char initialShape[4][4] = {
            {' ', ' ', ' ', ' '},
            {'J', ' ', ' ', ' '},
            {'J', 'J', 'J', ' '},
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