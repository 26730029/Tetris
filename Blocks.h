#ifndef BLOCKS_H
#define BLOCKS_H

// ================================================================
// Base class cho tat ca cac loai khoi Tetris (Tetromino).
// Cac class con (IBlock, OBlock, TBlock, SBlock, ZBlock, JBlock, LBlock)
// se ke thua tu class nay -- moi nguoi tu viet class con cua minh
// trong file rieng (vi du IBlock.h), chi can #include "Blocks.h".
//
// QUAN TRONG:
// - shape[4][4] la du lieu RIENG cua tung object, khong dung chung
//   nhu mang blocks[][4][4] cu -- moi khi spawn 1 khoi moi, ta tao
//   1 object moi (new IBlock(), new OBlock()...), nen khong bao gio
//   bi loi "khoi spawn sau bi sai hinh dang" nhu ban cu nua.
// - rotate() la ham AO (virtual) -- day chinh la tinh DA HINH
//   (polymorphism): khi goi currentBlock->rotate() thong qua con tro
//   Blocks*, C++ tu dong goi dung phien ban rotate() cua class con
//   thuc te (vi du OBlock override thanh ham rong vi khoi O xoay
//   khong doi hinh dang).
// ================================================================

class Blocks
{
protected:
    char shape[4][4];

public:
    Blocks()
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = ' ';
    }

    // Destructor PHAI la virtual -- vi ta se xoa object thong qua
    // con tro lop cha (Blocks*), neu khong co virtual o day, destructor
    // cua lop con se khong duoc goi -> memory leak / undefined behavior.
    virtual ~Blocks() {}

    // Xoay ma tran 4x4 mot goc 90 do theo chieu kim dong ho.
    // Day la hanh vi MAC DINH, dung chung cho hau het cac loai khoi
    // (I, T, S, Z, J, L). Lop con nao can hanh vi khac (vi du OBlock)
    // thi override lai ham nay.
    virtual void rotate()
    {
        char temp[4][4];
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                temp[j][3 - i] = shape[i][j];
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = temp[i][j];
    }

    // Doc 1 o trong ma tran hinh dang (dung cho canMove/block2Board/...).
    char get(int i, int j) const { return shape[i][j]; }

    // Ghi 1 o trong ma tran hinh dang (dung khi can revert lai sau khi
    // xoay ma vi tri moi khong hop le -- xem rotateCurrentBlock() trong
    // main.cpp).
    void set(int i, int j, char c) { shape[i][j] = c; }
};

#endif
