#include <iostream>
#include <cstdlib>   // rand(), srand(), system()
#include <ctime>     // time()
#include <unistd.h>  // usleep(), read()
#include <termios.h> // cau hinh terminal cho kbhit/getch
#include <fcntl.h>   // fcntl() - doc phim khong block
#include <csignal>   // xu ly Ctrl+C (SIGINT) an toan

using namespace std;

#define H 20
#define W 15

char board[H][W] = {};

int x, y, b;

// 7 loai khoi Tetris chuan: I, O, T, S, Z, J, L
char blocks[][4][4] = {
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

const int NUM_BLOCKS = 7;

// Hinh dang cua khoi DANG ROI hien tai (co the bi xoay).
// Duoc COPY tu blocks[b] moi khi spawn khoi moi -- khong bao gio
// tham chieu truc tiep vao blocks[][4][4], de tranh lam hong khuon mau goc.
char currentShape[4][4];

// ---- kbhit() / getch() thay the cho conio.h tren Linux ----
struct termios oldt, newt;

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

// neu nguoi choi bam Ctrl+C (SIGINT) de thoat gap trong luc dang choi,
// atexit() KHONG duoc goi -- terminal se bi ket o che do raw, nhin
// giong nhu bi "crash"/treo. Dang ky handler nay de dam bao terminal
// luon duoc tra ve trang thai cu truoc khi thoat, du thoat bang cach nao.
void handleSignal(int)
{
    resetTermios();
    exit(0);
}

int kbhit()
{
    unsigned char ch;
    int nread = read(STDIN_FILENO, &ch, 1);
    if (nread == 1)
    {
        ungetc(ch, stdin); // "tra lai" ky tu de getch() doc duoc
        return 1;
    }
    return 0;
}

char getch()
{
    char ch;
    ch = getchar();
    return ch;
}
// -------------------------------------------------------------

// forward declaration -- canMove() duoc dinh nghia ben duoi
// nhung rotateCurrentBlock() can goi den no truoc do.
bool canMove(int dx, int dy);

// Sinh khoi moi: COPY hinh dang tu khuon mau blocks[b] sang currentShape,
// khong bao gio thao tac truc tiep tren blocks[][4][4].
void spawnBlock()
{
    b = rand() % NUM_BLOCKS;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            currentShape[i][j] = blocks[b][i][j];
    x = 5;
    y = 0;
}

void rotateCurrentBlock()
{
    char temp[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            temp[j][3 - i] = currentShape[i][j];
        }
    }

    char old[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            old[i][j] = currentShape[i][j];
        }
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            currentShape[i][j] = temp[i][j];
        }
    }

    if (!canMove(0, 0))
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                currentShape[i][j] = old[i][j];
            }
        }
    }
}

bool canMove(int dx, int dy)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (currentShape[i][j] != ' ')
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
            if (currentShape[i][j] != ' ')
                board[y + i][x + j] = currentShape[i][j];
}

void boardDelBlock()
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (currentShape[i][j] != ' ')
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
    system("clear"); // "cls" la lenh Windows, tren Linux phai dung "clear"
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

int sleepTime = 500;
int totalLinesCleared = 0;
const int FRAME_MS = 16; // ~60 FPS -- tan suat doc phim, KHONG lien quan toi toc do roi

int main()
{
    initTermios();                 // bat che do doc phim khong block cho terminal
    atexit(resetTermios);          // dam bao terminal duoc tra ve binh thuong khi thoat
    signal(SIGINT, handleSignal);  // Ctrl+C
    signal(SIGTERM, handleSignal); // kill

    srand((unsigned int)time(0));
    spawnBlock();
    initBoard();

    int elapsedTime = 0; // bo dem thoi gian rieng cho gravity (roi xuong)
    bool quit = false;

    while (!quit)
    {
        boardDelBlock();

        // doc HET tat ca phim dang cho trong hang doi, khong chi 1 phim/vong lap
        while (kbhit())
        {
            char c = getch();
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
                    totalLinesCleared += removedLines;              // cong don hang da xoa
                    sleepTime = 500 - (totalLinesCleared / 5) * 50; // cu 5 hang xoa duoc thi giam 50ms
                    if (sleepTime < 100)
                    {
                        sleepTime = 100; // gioi han toc do khong nho hon 100ms
                    }
                }
                spawnBlock();
            }
        }

        block2Board();
        draw();
        usleep(FRAME_MS * 1000); // luon ngu dung 16ms/frame, KHONG phu thuoc sleepTime nua
    }
    return 0;
}