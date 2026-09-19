#ifndef TEMP_BLOCKS_H
#define TEMP_BLOCKS_H

#include "Blocks.h"

// ================================================================
// !!! TAM THOI (PLACEHOLDER) !!!
//
// File nay chi de Khang test duoc ngay refactor Blocks/canMove/...
// trong luc cho Hieu (IBlock+OBlock), Diem (ZBlock+JBlock),
// Thu (TBlock+SBlock), Khanh (LBlock) viet class that cua ho.
//
// KHI 4 nhanh feature/blocks-oop cua moi nguoi da duoc merge vao main:
// - XOA het noi dung file nay (hoac xoa han file TempBlocks.h)
// - Doi #include "TempBlocks.h" trong main.cpp thanh include cac file
//   that: IBlock.h, OBlock.h, TBlock.h, SBlock.h, ZBlock.h, JBlock.h, LBlock.h
// - Sua ham createBlock() trong main.cpp de new dung ten class that
//   (IBlock, OBlock, TBlock...) thay vi GenericBlock nhu duoi day.
//
// Day la phan viec cua Khanh trong card "Test tich hop toan bo".
// ================================================================

class GenericBlock : public Blocks
{
public:
    GenericBlock(const char init[4][4])
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                set(i, j, init[i][j]);
    }
};

// Du lieu hinh dang tam thoi -- giong het mang blocks[][4][4] cu,
// chi dung de GenericBlock khoi tao trong luc cho class that.
static const char TEMP_BLOCK_DATA[7][4][4] = {
    {{' ', ' ', 'I', ' '},
     {' ', ' ', 'I', ' '},
     {' ', ' ', 'I', ' '},
     {' ', ' ', 'I', ' '}},

    {{' ', ' ', ' ', ' '},
     {' ', 'O', 'O', ' '},
     {' ', 'O', 'O', ' '},
     {' ', ' ', ' ', ' '}},

    {{' ', ' ', ' ', ' '},
     {' ', 'T', ' ', ' '},
     {'T', 'T', 'T', ' '},
     {' ', ' ', ' ', ' '}},

    {{' ', ' ', ' ', ' '},
     {' ', 'S', 'S', ' '},
     {'S', 'S', ' ', ' '},
     {' ', ' ', ' ', ' '}},

    {{' ', ' ', ' ', ' '},
     {'Z', 'Z', ' ', ' '},
     {' ', 'Z', 'Z', ' '},
     {' ', ' ', ' ', ' '}},

    {{' ', ' ', ' ', ' '},
     {'J', ' ', ' ', ' '},
     {'J', 'J', 'J', ' '},
     {' ', ' ', ' ', ' '}},

    {{' ', ' ', ' ', ' '},
     {' ', ' ', 'L', ' '},
     {'L', 'L', 'L', ' '},
     {' ', ' ', ' ', ' '}}};

#endif