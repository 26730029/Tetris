#include <iostream>
#include <cstdlib>      // rand(), srand(), system()
#include <ctime>        // time()
#include <unistd.h>     // usleep(), read()
#include <termios.h>    // cau hinh terminal cho kbhit/getch
#include <fcntl.h>      // fcntl() - doc phim khong block

using namespace std;

#define H 20
#define W 15

char board[H][W] = {};

int x, y, b;

// 7 loai khoi Tetris chuan: I, O, T, S, Z, J, L
char blocks[][4][4] = {
        {{' ',' ','I',' '},
         {' ',' ','I',' '},
         {' ',' ','I',' '},
         {' ',' ','I',' '}},

        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},

        {{' ',' ',' ',' '},
         {' ','T',' ',' '},
         {'T','T','T',' '},
         {' ',' ',' ',' '}},

        {{' ',' ',' ',' '},
         {' ','S','S',' '},
         {'S','S',' ',' '},
         {' ',' ',' ',' '}},

        {{' ',' ',' ',' '},
         {'Z','Z',' ',' '},
         {' ','Z','Z',' '},
         {' ',' ',' ',' '}},

        {{' ',' ',' ',' '},
         {'J',' ',' ',' '},
         {'J','J','J',' '},
         {' ',' ',' ',' '}},

        {{' ',' ',' ',' '},
         {' ',' ','L',' '},
         {'L','L','L',' '},
         {' ',' ',' ',' '}}
};

const int NUM_BLOCKS = 7;

// ---- kbhit() / getch() thay the cho conio.h tren Linux ----
struct termios oldt, newt;

void initTermios(){
    tcgetattr(STDIN_FILENO, &oldt);          // luu cau hinh terminal cu
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);        // tat che do "line buffered" va echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK); // doc khong block
}

void resetTermios(){
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // tra terminal ve trang thai cu
}

int kbhit(){
    unsigned char ch;
    int nread = read(STDIN_FILENO, &ch, 1);
    if (nread == 1){
        ungetc(ch, stdin); // "tra lai" ky tu de getch() doc duoc
        return 1;
    }
    return 0;
}

char getch(){
    char ch;
    ch = getchar();
    return ch;
}
// -------------------------------------------------------------

bool canMove(int dx, int dy){
    for (int i = 0; i < 4; i++ )
        for (int j = 0; j < 4; j++ )
            if (blocks[b][i][j] != ' ') {
                int xt = x + j + dx;
                int yt = y + i + dy;
                if (xt < 1 || xt >= W-1 || yt >= H-1 ) return false;
                if (board[yt][xt] != ' ') return false;
            }
    return true;
}

void block2Board(){
    for (int i = 0; i < 4; i++ )
        for (int j = 0; j < 4; j++ )
            if (blocks[b][i][j] != ' ')
                board[y+i][x+j] = blocks[b][i][j];
}

void boardDelBlock(){
    for (int i = 0; i < 4; i++ )
        for (int j = 0; j < 4; j++ )
            if (blocks[b][i][j] != ' ')
                board[y+i][x+j] = ' ';
}

void initBoard(){
    for (int i = 0 ; i < H ; i++)
        for (int j = 0 ; j < W ; j++)
            if (i == 0 || i == H-1 || j ==0 || j == W-1) board[i][j] = '#';
            else board[i][j] = ' ';
}

void draw(){
    system("clear");

    for (int i = 0 ; i < H ; i++, cout<<endl)
        for (int j = 0 ; j < W ; j++) cout<<board[i][j];

    cout << "Total lines cleared: " << totalLinesCleared << endl;
    cout << "Current speed (sleepTime): " << sleepTime << "ms" << endl;
}

int removeLine(){
    int removedLines = 0;

    // Duyet tu hang duoi cung len tren
    for (int i = H - 2; i >= 1; i--) {

        // Kiem tra hang i co day hay khong
        bool full = true;

        for (int j = 1; j < W - 1; j++) {
            if (board[i][j] == ' ') {
                full = false;
                break;
            }
        }

        // Neu hang day -> xoa hang
        if (full) {

            // Don tat ca cac hang phia tren xuong 1 bac
            for (int row = i; row > 1; row--) {
                for (int col = 1; col < W - 1; col++) {
                    board[row][col] = board[row - 1][col];
                }
            }

            // Hang 1 tro thanh hang trong
            for (int col = 1; col < W - 1; col++) {
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

int main() {
    initTermios();          // bat che do doc phim khong block cho terminal
    atexit(resetTermios);   // dam bao terminal duoc tra ve binh thuong khi thoat

    srand((unsigned int)time(0));
    x = 5; y = 0; b = rand() % NUM_BLOCKS;
    initBoard();
    while (1){
        boardDelBlock();
        if (kbhit()){
            char c = getch();
            if (c == 'a' && canMove(-1,0)) x--;
            if (c == 'd' && canMove( 1,0)) x++;
            if (c == 'x' && canMove( 0,1)) y++;
            if (c == 'q') break;
        }
        if (canMove(0,1)) y++;
        else{
            block2Board();
            // int removedLines = removeLine();
            // x = 5; y = 0; b = rand() % NUM_BLOCKS;
            int removedLines = removeLine();
            // them doan logic tang toc
            if (removedLines > 0) {
                totalLinesCleared += removedLines; // cong don hang da xoa
                sleepTime = 500 - (totalLinesCleared / 5) * 50; // cu 5 hang xoa duoc thi giam 50ms thoi gian roi
                // goi han toc do khong nho hon 100ms
                if (sleepTime < 100) {
                    sleepTime = 100;
                }
            }
            x = 5; y = 0; b = rand() % NUM_BLOCKS;

        }
        block2Board();
        draw();
       // usleep(500 * 1000); // usleep tinh bang micro-giay, nen 500ms = 500*1000
        usleep(sleepTime * 1000); 
    }
    return 0;
}