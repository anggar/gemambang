#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define REP(i, k) for(int i=0; i<k; i++)
#define REPP(i, j, k) REP(i, k) REP(j, k)
#define GRT(i) (i >= 0)                                                      // Boundary handling
#define LST(i) (i < WIDTH)                                                   // Boundary handling
#define WIDTH 8

#define SCRH 24+3
#define SCRW 48+1

int Board[WIDTH][WIDTH];
int FloodedHorizontally[WIDTH][WIDTH];
int FloodedVertically[WIDTH][WIDTH];

long long Score = 0;

// BASIC FUNCTION

void SetupWindow(){
    system("mode 49,27");
    COORD bufferSize = {SCRW, SCRH};
    SMALL_RECT WinRect   = {0, 0, SCRW, SCRH};
    SMALL_RECT * WinSize = &WinRect;

    SetConsoleTitle("Gemambang");

    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), 1, WinSize);

    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize);
}

void GotoXY(SHORT x, SHORT y){
    COORD K = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), K);
}

void SetColor(SHORT color){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void Delay(int ms){
    clock_t start = clock();

    while(clock() < start + ms);
}

// INTERFACE

void Box(SHORT x, SHORT y, SHORT id){
    SHORT KoorX = (6*x) + 1;
    SHORT KoorY = (3*y) + 1;

    SetColor(id);
    if(id < 0) SetColor(0);
    GotoXY(KoorX, KoorY+0); printf("%c%c%c%c%c", 222, 219, 219, 219, 221);
    GotoXY(KoorX, KoorY+1); printf("%c%c%c%c%c", 222, 219, 219, 219, 221);
}

void SelectionBox(SHORT x, SHORT y){
    SHORT KoorX = 6*x;    
    SHORT KoorY = 3*y;

    SetColor(9);
    GotoXY(KoorX, KoorY+0);                                              printf("%c%c%c%c%c%c%c\n",  201, 205, 205, 205, 205, 205, 187);
    GotoXY(KoorX, KoorY+1); printf("%c", 186); GotoXY(KoorX+6, KoorY+1); printf(            "%c\n",                                186);
    GotoXY(KoorX, KoorY+2); printf("%c", 186); GotoXY(KoorX+6, KoorY+2); printf(            "%c\n",                                186);
    GotoXY(KoorX, KoorY+3);                                              printf("%c%c%c%c%c%c%c\n",  200, 205, 205, 205, 205, 205, 188);
}

void ClearSelectionBox(SHORT x, SHORT y){
    SHORT KoorX = 6*x;    
    SHORT KoorY = 3*y;

    GotoXY(KoorX, KoorY+0);                                             printf("%c%c%c%c%c%c%c\n", 32, 32, 32, 32, 32, 32, 32);
    GotoXY(KoorX, KoorY+1); printf("%c", 32); GotoXY(KoorX+6, KoorY+1); printf(            "%c\n",                         32);
    GotoXY(KoorX, KoorY+2); printf("%c", 32); GotoXY(KoorX+6, KoorY+2); printf(            "%c\n",                         32);
    GotoXY(KoorX, KoorY+3);                                             printf("%c%c%c%c%c%c%c\n", 32, 32, 32, 32, 32, 32, 32);
}

void SelectedSelectionBox(SHORT x, SHORT y){
    SHORT KoorX = 6*x;    
    SHORT KoorY = 3*y;  

    SetColor(15);
    GotoXY(KoorX, KoorY+0);                                              printf("%c%c%c%c%c%c%c\n",  218, 196, 196, 196, 196, 196, 191);
    GotoXY(KoorX, KoorY+1); printf("%c", 179); GotoXY(KoorX+6, KoorY+1); printf(            "%c\n",                                179);
    GotoXY(KoorX, KoorY+2); printf("%c", 179); GotoXY(KoorX+6, KoorY+2); printf(            "%c\n",                                179);
    GotoXY(KoorX, KoorY+3);                                              printf("%c%c%c%c%c%c%c\n",  192, 196, 196, 196, 196, 196, 217);
}

// RANDOM GENERATION

int GenerateRandomNumber(){
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    /* using nano-seconds instead of seconds */
    srand((time_t)ts.tv_nsec);
    
    return (rand()%5)+1;
}

void GenerateRandomBoard(){
    srand(time(0));

    REPP(x, y, WIDTH){
        Board[x][y] = ((x+y)*rand())%5 + 1;
    }
}

void GenerateBox(){
    REPP(x, y, WIDTH){
        Box(x, y, Board[x][y]);
    }
}

void SwapInt(int &a, int &b){
    int t = b;
    b = a;
    a = t;
}

// FLOODING ALGORITHM

int HorizontalFlood(SHORT x, SHORT y, int flood, int fill){
    if(FloodedHorizontally[x][y] != flood) return 0;

    FloodedHorizontally[x][y] = fill;

    int RIGHT = 0;
    int HERE  = 1;

    if(LST(x+1)) RIGHT = HorizontalFlood(x+1, y, flood, fill);

    return RIGHT+HERE;
}

int BoundedHorizontalFlood(SHORT x, SHORT y, int flood, int fill, int boundary){
    if(FloodedHorizontally[x][y] != flood) return 0;

    int ret = HorizontalFlood(x, y, flood, fill);

    if(ret < boundary){ HorizontalFlood(x, y, fill, flood); ret = 0;}

    return ret;
}

int ExhaustiveHorizontalFlood(int flood, int fill, int boundary){
    int ret = 0;

    REPP(i, j, WIDTH) FloodedHorizontally[i][j] = Board[i][j];
    REPP(y, x, WIDTH) ret += BoundedHorizontalFlood(x, y, flood, fill, boundary);

    return ret;
}

int VerticalFlood(SHORT x, SHORT y, int flood, int fill){
    if(FloodedVertically[x][y] != flood) return 0;

    FloodedVertically[x][y] = fill;

    int DOWN = 0;
    int HERE  = 1;

    if(LST(y+1)) DOWN = VerticalFlood(x, y+1, flood, fill);

    return DOWN+HERE;
}

int BoundedVerticalFlood(SHORT x, SHORT y, int flood, int fill, int boundary){
    if(FloodedVertically[x][y] != flood) return 0;

    int ret = VerticalFlood(x, y, flood, fill);

    if(ret < boundary){ VerticalFlood(x, y, fill, flood); ret=0;}

    return ret;
}

int ExhaustiveVerticalFlood(int flood, int fill, int boundary){
    int ret = 0;

    REPP(i, j, WIDTH) FloodedVertically[i][j] = Board[i][j];
    REPP(y, x, WIDTH) ret += BoundedVerticalFlood(x, y, flood, fill, boundary);

    return ret;
}

int ExhaustiveFlood(int flood, int fill, int boundary){
    int ret = 0;

    ret += ExhaustiveHorizontalFlood(flood, fill, boundary);
    ret += ExhaustiveVerticalFlood(flood, fill, boundary);

    REPP(i, j, WIDTH){
        if((FloodedHorizontally[i][j] < 0) || (FloodedVertically[i][j] < 0)){
            Board[i][j] = fill;
        }
    }

    return ret;
}

void SteppedFall(int ms){                                                                           // Unlike other functions, It can generate board
    int CenterHole = 1;

    int GoodToSwap[WIDTH][WIDTH] = {0};

    while(CenterHole){
        CenterHole = 0;
        REPP(y, x, WIDTH){
            GoodToSwap[x][y] = 0;

            if(GRT(y-1) && ((Board[x][y] < 0) && (Board[x][y-1] > 0))){
                CenterHole = 1;
                GoodToSwap[x][y] = 1;
            }
        }

        REPP(y, x, WIDTH){
            if(GoodToSwap[x][y]) SwapInt(Board[x][y], Board[x][y-1]);
        }

        if(ms) GenerateBox();
        Delay(ms);
    }
}

void FillEmptyBox(int ms){
    int StillEmpty;

    do {
        StillEmpty = 0;

        REP(x, WIDTH){
            if(Board[x][0] < 0){ StillEmpty = 1; Board[x][0] = GenerateRandomNumber();}
        }

        SteppedFall(ms);
    } while(StillEmpty);
}

void FillItBlue(SHORT x, SHORT y){
    Board[x][y] = 1;
    GenerateBox();
}

void KeepItClean(){
    int check = 1;

    while(check != 0){
        check = 0;
        REP(i, 5) check += ExhaustiveFlood(i+1, (i+1)*(-1), 3);

        GenerateBox();
        SteppedFall(75);
        FillEmptyBox(25);

        Score += check;
    }

    GotoXY(0, 25);
    printf("                    \n");
    GotoXY(0, 25);
    printf("SCORE L %lld\n", Score);
}

void FloodAll(){
    int ret = 0;

    REP(i, 5) ret += ExhaustiveFlood(i+1, (i+1)*(-1), 3);

    GenerateBox();
    SteppedFall(75);
    FillEmptyBox(25);

    KeepItClean();

    Score += ret;

    GotoXY(0, 24);
    printf("                    \n");
    GotoXY(0, 24);
    printf("POINT : %lld\n", ret);
}

int PreSwap(SHORT x, SHORT y){
    SelectedSelectionBox(x, y);

    SHORT SwappedCursorX = x;
    SHORT SwappedCursorY = y;

    fflush(stdin);

    int Ret = 0;

    if(getch() == 224){
        switch(getch()){
            case 75:
                if(!SwappedCursorX) break;          // Left Arrow 
                SwappedCursorX--;
                Ret = 1;
                break;
            case 72:
                if(!SwappedCursorY) break;          // Up Arrow 
                SwappedCursorY--;
                Ret = 2;
                break;
            case 77:
                if(SwappedCursorX == WIDTH-1) break;      // Right Arrow 
                SwappedCursorX++;
                Ret = 3;
                break;
            case 80:
                if(SwappedCursorY == WIDTH-1) break;      // Down Arrow 
                SwappedCursorY++;
                Ret = 4;
                break;
        }
    }

    SwapInt(Board[x][y], Board[SwappedCursorX][SwappedCursorY]);

    Box(x, y, Board[x][y]);
    Box(SwappedCursorX, SwappedCursorY, Board[SwappedCursorX][SwappedCursorY]);

    ClearSelectionBox(x, y);
    SelectionBox(SwappedCursorX, SwappedCursorY);

    FloodAll();;

    return Ret;
}

void Movement(){
    char Key;   
    SHORT SelectionCursorX = 0;
    SHORT SelectionCursorY = 0;
    SHORT Pressed = 0;

    while(1){
        GotoXY(0, 3*8 + 1);
        switch(getch()){
            case 32:
                Pressed ^= 1;
                break;
            case 'g':                           // For Testing
                GenerateRandomBoard();
                GenerateBox();
                break;        
            case 'b':
                FillItBlue(SelectionCursorX, SelectionCursorY);
                break;
            case 'q':
                ExhaustiveFlood(1, -1, 3);
                ExhaustiveFlood(2, -2, 3);
                ExhaustiveFlood(3, -3, 3);
                ExhaustiveFlood(4, -4, 3);
                ExhaustiveFlood(5, -5, 3);
                GenerateBox();
                Delay(250);
                SteppedFall(75);
                FillEmptyBox(25);
                break;
            case 224:
                switch(getch()){
                    case 75:
                        ClearSelectionBox(SelectionCursorX, SelectionCursorY);
                        if(SelectionCursorX == 0)  break;
                        SelectionCursorX--;
                        break;
                    case 72:
                        ClearSelectionBox(SelectionCursorX, SelectionCursorY);
                        if(SelectionCursorY == 0) break;
                        SelectionCursorY--;
                        break;
                    case 80:
                        ClearSelectionBox(SelectionCursorX, SelectionCursorY);
                        if(SelectionCursorY == WIDTH-1) break;
                        SelectionCursorY++;
                        break;
                    case 77:
                        ClearSelectionBox(SelectionCursorX, SelectionCursorY);
                        if(SelectionCursorX == WIDTH-1) break;
                        SelectionCursorX++;
                        break;
                break;
            }
        }

        SelectionBox(SelectionCursorX, SelectionCursorY);

        if(Pressed){
            ClearSelectionBox(SelectionCursorX, SelectionCursorY);

            switch(PreSwap(SelectionCursorX, SelectionCursorY)){
                case 1:
                    SelectionCursorX--;
                    break;
                case 2:
                    SelectionCursorY--;
                    break;
                case 3:
                    SelectionCursorX++;
                    break;
                case 4:
                    SelectionCursorY++;
                    break;
                case 0:
                    break;
            }

            SelectionBox(SelectionCursorX, SelectionCursorY);
            Pressed ^= 1;
        }
    }
}

void GenerateCleanBox(){
    GenerateRandomBoard();

    int check;

    do {
        check = 0;
        REP(i, 5) check += ExhaustiveFlood(i+1, (i+1)*(-1), 3);

        SteppedFall(0);
        FillEmptyBox(0);
    } while(check);
}

int main(){
    SetupWindow();

    system("cls");

    GenerateCleanBox();
    GenerateBox();

    Movement();

    if(getch()){
        GenerateBox();
    }
}