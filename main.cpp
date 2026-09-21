#include <iostream>
#include <cstdlib> // rand(), srand(), system()
#include <ctime>   // time()
#include <chrono>  // std::chrono::milliseconds
#include <thread>  // std::this_thread::sleep_for
#include <csignal> // xu ly Ctrl+C (SIGINT) an toan
#include <fstream> // ifstream/ofstream - doc/ghi highscore.txt

#ifdef _WIN32
#include <conio.h>   // _kbhit(), _getch()
#include <windows.h> // Sleep()
#else
#include <unistd.h>  // usleep(), read()
#include <termios.h> // cau hinh terminal cho kbhit/getch
#include <fcntl.h>   // fcntl() - doc phim khong block
#endif

#include "Blocks.h"
// TAM THOI: xem ghi chu trong TempBlocks.h -- se bi thay the boi
// IBlock.h, OBlock.h, TBlock.h, SBlock.h, ZBlock.h, JBlock.h, LBlock.h
// that su khi Khanh merge xong tat ca cac nhanh cua nhom.
#include "TempBlocks.h"
#include "LBlock.h"

using namespace std;

#define H 20
#define W 15

char board[H][W] = {};

int x, y, b;
int score = 0;
int highScore = 0;

const int NUM_BLOCKS = 2;

// Con tro toi khoi DANG ROI hien tai. Moi lan spawn, ta tao 1 OBJECT
// MOI (new IBlock()/new OBlock()/...) thay vi copy tu 1 mang khuon
// mau dung chung -- nen khong con kha nang bi "hong khuon mau goc"
// nhu bug cu (xem lai lich su commit currentShape refactor).
Blocks *currentBlock = nullptr;

// Factory: tao 1 object khoi moi dung loai (0..1), tra ve qua con tro
// lop cha Blocks*.
Blocks *createBlock(int type)
{
    if (type == 0)
        return new IBlock();
    return new OBlock();
}

// ---- Input khong blocking, dung duoc tren Windows va macOS ----
#ifdef _WIN32

void initTermios() {}
void resetTermios() {}

int kbhit()
{
    return _kbhit();
}

char readKey()
{
    return static_cast<char>(_getch());
}

#else

struct termios oldt, newt;
int pendingKey = -1;

void initTermios()
{
    tcgetattr(STDIN_FILENO, &oldt); // luu cau hinh terminal cu
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // tat che do "line buffered" va echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK); // doc khong block
}

void resetTermios()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // tra terminal ve trang thai cu
}

int kbhit()
{
    unsigned char ch;
    int nread = read(STDIN_FILENO, &ch, 1);
    if (nread == 1)
    {
        pendingKey = ch;
        return 1;
    }
    return 0;
}

char readKey()
{
    char ch = static_cast<char>(pendingKey);
    pendingKey = -1;
    return ch;
}

#endif

// Dam bao terminal duoc tra ve trang thai cu khi ket thuc dot ngot.
void handleSignal(int)
{
    resetTermios();
    exit(0);
}

void sleepMilliseconds(int milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
// -------------------------------------------------------------

bool canMove(int dx, int dy);

// Sinh khoi moi: tao object moi qua factory, giai phong khoi cu truoc
// de tranh memory leak (moi lan spawn deu new 1 object, neu khong
// delete khoi cu se bi leak dan theo thoi gian choi).
void spawnBlock()
{
    delete currentBlock;
    b = rand() % NUM_BLOCKS;
    currentBlock = createBlock(b);
    x = 5;
    // y=1 (khong phai 0): IBlock dung chieu doc chiem du ca 4 hang cua
    // luoi 4x4 (hang 0..3), khong duoc dem hang trong nhu OBlock. Neu
    // spawn o y=0, hang 0 cua IBlock roi dung vao hang tuong tren cung
    // (board[0][..]='#'), khien canMove(0,0) luon bao va cham va Game
    // Over oan ngay khi IBlock xuat hien, du board con trong hoan toan.
    y = 1;
}

// Xoay khoi dang roi. currentBlock->rotate() la mot loi goi DA HINH:
// C++ tu dong chay dung phien ban rotate() cua class con thuc te
// (vi du OBlock se khong lam gi ca, cac khoi khac dung ham xoay mac
// dinh trong Blocks). Neu vi tri sau khi xoay khong hop le, revert
// lai bang get()/set() (khong can biet class con la gi).
void rotateCurrentBlock()
{
    char old[4][4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            old[i][j] = currentBlock->get(i, j);

    currentBlock->rotate();

    if (!canMove(0, 0))
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                currentBlock->set(i, j, old[i][j]);
    }
}

bool canMove(int dx, int dy)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (currentBlock->get(i, j) != ' ')
            {
                int xt = x + j + dx;
                int yt = y + i + dy;
                if (xt < 1 || xt >= W - 1 || yt >= H - 1)
                    return false;
                if (board[yt][xt] != ' ')
                    return false;
            }
    return true;
}

void block2Board()
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (currentBlock->get(i, j) != ' ')
                board[y + i][x + j] = currentBlock->get(i, j);
}

void boardDelBlock()
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (currentBlock->get(i, j) != ' ')
                board[y + i][x + j] = ' ';
}

void initBoard()
{
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            if (i == 0 || i == H - 1 || j == 0 || j == W - 1)
                board[i][j] = '#';
            else
                board[i][j] = ' ';
}

void draw()
{
    clearScreen();
    for (int i = 0; i < H; i++, cout << endl)
    {
        for (int j = 0; j < W; j++)
        {
            if (board[i][j] == ' ')
            {
                cout << "  "; // In 2 khoang trang cho o trong
            }
            else if (board[i][j] == '#')
            {
                cout << "##"; // In 2 ky tu # de tao thanh hinh vuong lam tuong
            }
            else
            {
                cout << "[]"; // 2 ky tu dai dien cho khoi gach
            }
        }

        if (i == 1)
        {
            cout << "  Score: " << score;
        }
        else if (i == 2)
        {
            cout << "  High: " << highScore;
        }
    }
}

int removeLine()
{
    int removedLines = 0;

    // Duyet tu hang duoi cung len tren
    for (int i = H - 2; i >= 1; i--)
    {

        // Kiem tra hang i co day hay khong
        bool full = true;

        for (int j = 1; j < W - 1; j++)
        {
            if (board[i][j] == ' ')
            {
                full = false;
                break;
            }
        }

        // Neu hang day -> xoa hang
        if (full)
        {

            // Don tat ca cac hang phia tren xuong 1 bac
            for (int row = i; row > 1; row--)
            {
                for (int col = 1; col < W - 1; col++)
                {
                    board[row][col] = board[row - 1][col];
                }
            }

            // Hang 1 tro thanh hang trong
            for (int col = 1; col < W - 1; col++)
            {
                board[1][col] = ' ';
            }

            removedLines++;

            i++;
        }
    }

    return removedLines;
}

const char *HIGHSCORE_FILE = "highscore.txt";

// Doc diem cao tu file. Neu file chua ton tai (lan dau choi), is_open()
// tra ve false va ham tra ve mac dinh 0.
int loadHighScore()
{
    int highScore = 0;
    ifstream inFile(HIGHSCORE_FILE);
    if (inFile.is_open())
    {
        inFile >> highScore;
        inFile.close();
    }
    return highScore;
}

// Ghi de diem cao moi vao file (ofstream mac dinh la che do ghi de/truncate).
void saveHighScore(int highScore)
{
    ofstream outFile(HIGHSCORE_FILE);
    if (outFile.is_open())
    {
        outFile << highScore;
        outFile.close();
    }
}

int sleepTime = 250; // toc do roi ban dau (ms/hang) -- giam tu 500 de khoi roi nhanh hon
int totalLinesCleared = 0;
const int FRAME_MS = 16; // ~60 FPS -- tan suat doc phim, KHONG lien quan toi toc do roi

int main()
{
    initTermios();                 // bat che do doc phim khong block cho terminal
    atexit(resetTermios);          // dam bao terminal duoc tra ve binh thuong khi thoat
    signal(SIGINT, handleSignal);  // Ctrl+C
    signal(SIGTERM, handleSignal); // kill

    srand((unsigned int)time(0));
    highScore = loadHighScore();
    // initBoard() phai chay TRUOC spawnBlock(): canMove() doc board[][] de biet
    // o nao trong, neu board chua init (toan '\0') thi canMove se tra ve false
    // ngay tu dau va bi bao Game Over oan.
    initBoard();
    spawnBlock();

    int elapsedTime = 0; // bo dem thoi gian rieng cho gravity (roi xuong)
    bool quit = false;
    bool gameOver = false; // phan biet thoat do Game Over voi thoat do bam 'q'

    if (!canMove(0, 0))
    {
        quit = true;
        gameOver = true;
    }

    while (!quit)
    {
        boardDelBlock();

        // doc HET tat ca phim dang cho trong hang doi, khong chi 1 phim/vong lap
        while (kbhit())
        {
            char c = readKey();
            if (c == 'w')
                rotateCurrentBlock();
            if (c == 'a' && canMove(-1, 0))
                x--;
            if (c == 'd' && canMove(1, 0))
                x++;
            if (c == 'x' && canMove(0, 1))
                y++;
            if (c == 'q')
            {
                quit = true;
                break;
            }
        }
        if (quit)
            break;

        // gravity (roi xuong) chi kich hoat khi du thoi gian sleepTime da troi qua
        elapsedTime += FRAME_MS;
        if (elapsedTime >= sleepTime)
        {
            elapsedTime = 0;
            if (canMove(0, 1))
                y++;
            else
            {
                block2Board();
                int removedLines = removeLine();
                if (removedLines > 0)
                {
                    // Bang tinh diem chuan Tetris: Single/Double/Triple/Tetris
                    if (removedLines == 1)
                        score += 100;
                    else if (removedLines == 2)
                        score += 300;
                    else if (removedLines == 3)
                        score += 500;
                    else if (removedLines == 4)
                        score += 800;

                    totalLinesCleared += removedLines;              // cong don hang da xoa
                    sleepTime = 250 - (totalLinesCleared / 5) * 50; // cu 5 hang xoa duoc thi giam 50ms
                    if (sleepTime < 100)
                    {
                        sleepTime = 100; // gioi han toc do khong nho hon 100ms
                    }
                }
                spawnBlock();
                if (!canMove(0, 0))
                {
                    quit = true;
                    gameOver = true;
                }
            }
        }

        block2Board();
        draw();
        sleepMilliseconds(FRAME_MS); // luon ngu dung 16ms/frame, KHONG phu thuoc sleepTime nua
    }

    if (gameOver)
    {
        cout << "GAME OVER" << endl;
        cout << "Score: " << score << endl;

        if (score > highScore)
        {
            highScore = score;
            saveHighScore(highScore);
        }
    }

    delete currentBlock; // giai phong khoi cuoi cung truoc khi thoat
    return 0;
}