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

#define OFFSET (WIDTH*6 + 5 + 2)

#define SCRH 28
#define SCRW 90

int Board[WIDTH][WIDTH];
int FloodedHorizontally[WIDTH][WIDTH];
int FloodedVertically[WIDTH][WIDTH];

long long Score = 0;

// BASIC FUNCTION

void SetupWindow(){
    system("mode 90,30");
    COORD bufferSize = {SCRW, SCRH+2};
    SMALL_RECT WinRect   = {0, 0, SCRW, SCRH+2};
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

void SwapInt(int &a, int &b){
    int t = b;
    b = a;
    a = t;
}

// INTERFACE

void Box(SHORT x, SHORT y, SHORT id){
    SHORT KoorX = (6*x) + 1 + 3;
    SHORT KoorY = (3*y) + 1 + 1;

    SetColor(id);
    if(id < 0) SetColor(0);
    GotoXY(KoorX, KoorY+0); printf("%c%c%c%c%c", 222, 219, 219, 219, 221);
    GotoXY(KoorX, KoorY+1); printf("%c%c%c%c%c", 222, 219, 219, 219, 221);
}

void SelectionBox(SHORT x, SHORT y){
    SHORT KoorX = 6*x + 3;    
    SHORT KoorY = 3*y + 1;

    SetColor(9);
    GotoXY(KoorX, KoorY+0);                                              printf("%c%c%c%c%c%c%c\n",  201, 205, 205, 205, 205, 205, 187);
    GotoXY(KoorX, KoorY+1); printf("%c", 186); GotoXY(KoorX+6, KoorY+1); printf(            "%c\n",                                186);
    GotoXY(KoorX, KoorY+2); printf("%c", 186); GotoXY(KoorX+6, KoorY+2); printf(            "%c\n",                                186);
    GotoXY(KoorX, KoorY+3);                                              printf("%c%c%c%c%c%c%c\n",  200, 205, 205, 205, 205, 205, 188);
}

void ClearSelectionBox(SHORT x, SHORT y){
    SHORT KoorX = 6*x + 3;    
    SHORT KoorY = 3*y + 1;

    GotoXY(KoorX, KoorY+0);                                             printf("%c%c%c%c%c%c%c\n", 32, 32, 32, 32, 32, 32, 32);
    GotoXY(KoorX, KoorY+1); printf("%c", 32); GotoXY(KoorX+6, KoorY+1); printf(            "%c\n",                         32);
    GotoXY(KoorX, KoorY+2); printf("%c", 32); GotoXY(KoorX+6, KoorY+2); printf(            "%c\n",                         32);
    GotoXY(KoorX, KoorY+3);                                             printf("%c%c%c%c%c%c%c\n", 32, 32, 32, 32, 32, 32, 32);
}

void SelectedSelectionBox(SHORT x, SHORT y){
    SHORT KoorX = 6*x + 3;    
    SHORT KoorY = 3*y + 1;  

    SetColor(15);
    GotoXY(KoorX, KoorY+0);                                              printf("%c%c%c%c%c%c%c\n",  218, 196, 196, 196, 196, 196, 191);
    GotoXY(KoorX, KoorY+1); printf("%c", 179); GotoXY(KoorX+6, KoorY+1); printf(            "%c\n",                                179);
    GotoXY(KoorX, KoorY+2); printf("%c", 179); GotoXY(KoorX+6, KoorY+2); printf(            "%c\n",                                179);
    GotoXY(KoorX, KoorY+3);                                              printf("%c%c%c%c%c%c%c\n",  192, 196, 196, 196, 196, 196, 217);
}

void OuterBorder(){
    REP(i, WIDTH*6 + 5){
        GotoXY(i+1, 0);
        if(i == 0)          { printf("%c", 213); continue;}
        if(i == WIDTH*6+4)  { printf("%c", 184); continue;}
        printf("%c", 205);
    }

    REP(i, WIDTH*6 + 5){
        GotoXY(i+1, WIDTH*3 + 2);
        if(i == 0)          { printf("%c", 192); continue;}
        if(i == WIDTH*6+4)  { printf("%c", 217); continue;}
        printf("%c", 196);
    }

    REP(j, WIDTH*3 + 1){
        GotoXY(1, j+1);
        printf("%c", 179);

        GotoXY(WIDTH*6 + 5, j+1);
        printf("%c", 179);
    }
}

void SidepanelOne(){
    REP(i, 25){
        GotoXY(i+OFFSET, 0);
        if(i == 0)   { printf("%c", 213); continue;}
        if(i == 24)  { printf("%c", 184); continue;}
        printf("%c", 205);
    }

    REP(i, 25){
        GotoXY(i+OFFSET, 11);
        if(i == 0)   { printf("%c", 192); continue;}
        if(i == 24)  { printf("%c", 217); continue;}
        printf("%c", 196);
    }

    REP(j, 10){
        GotoXY(OFFSET, j+1);
        printf("%c", 179);

        GotoXY(OFFSET+24, j+1);
        printf("%c", 179);
    }
}

void MainMenu(){
    SidepanelOne();

    GotoXY(OFFSET, 2); printf("%c   G E M A M B A N G   %c", 179, 179);
    GotoXY(OFFSET, 4); printf("%c", 195); REP(i, 23) printf("%c", 196); printf("%c", 180);
    
    GotoXY(OFFSET, 6); printf("%c    NEW GAME           %c", 179, 179);
    GotoXY(OFFSET, 7); printf("%c    CREDIT             %c", 179, 179);
    GotoXY(OFFSET, 8); printf("%c    HELP               %c", 179, 179);
    GotoXY(OFFSET, 9); printf("%c    EXIT               %c", 179, 179);

    SHORT textPos = 0;

    while(1){
        switch(getch()){
            case 224:
                switch(getch()){
                    case 72:
                        GotoXY(OFFSET+3, textPos+6);
                        printf("%c", 32);
                        if(textPos != 0) textPos--;
                        break;
                    case 80:
                        GotoXY(OFFSET+3, textPos+6);
                        printf("%c", 32);
                        if(textPos != 3) textPos++;
                        break;
                }
                break;
            default: return;
        }

        GotoXY(OFFSET+3, textPos+6);
        printf("%c", 254);
    }
}

void HighScore(){
    REP(i, 25){
        GotoXY(i+OFFSET, 12);
        if(i == 0)   { printf("%c", 213); continue;}
        if(i == 24)  { printf("%c", 184); continue;}
        printf("%c", 205);
    }

    REP(i, 25){
        GotoXY(i+OFFSET, 26);
        if(i == 0)   { printf("%c", 192); continue;}
        if(i == 24)  { printf("%c", 217); continue;}
        printf("%c", 196);
    }

    REP(j, 13){
        GotoXY(OFFSET, j+13);
        printf("%c", 179);

        GotoXY(OFFSET+24, j+13);
        printf("%c", 179);
    }

    GotoXY(OFFSET, 2+12); printf("%c   H I G H S C O R E   %c", 179, 179);
    GotoXY(OFFSET, 4+12); printf("%c", 195); REP(i, 23) printf("%c", 196); printf("%c", 180);
    
    GotoXY(OFFSET, 6+12);  printf("%c  1. %d", 179, 352); GotoXY(OFFSET+24, 6+12);  printf("%c", 179);
    GotoXY(OFFSET, 8+12);  printf("%c  2. %d", 179, 352); GotoXY(OFFSET+24, 8+12);  printf("%c", 179);
    GotoXY(OFFSET, 10+12); printf("%c  3. %d", 179, 352); GotoXY(OFFSET+24, 10+12); printf("%c", 179);
    GotoXY(OFFSET, 12+12); printf("%c  4. %d", 179, 352); GotoXY(OFFSET+24, 12+12); printf("%c", 179);
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

// ADDITIONAL FUNCIONALITY

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

void KeepItClean(SHORT ms){
    int check = 1;

    while(check != 0){
        check = 0;
        REP(i, 5) check += ExhaustiveFlood(i+1, (i+1)*(-1), 3);

        GenerateBox();
        SteppedFall(3*ms);
        FillEmptyBox(ms);

        Score += check;
    }

    GotoXY(0, 25);
    printf("                    \n");
    GotoXY(0, 25);
    printf("SCORE L %lld\n", Score);
}

// MAIN FUNCTION

void FloodAll(){
    int ret = 0;

    REP(i, 5) ret += ExhaustiveFlood(i+1, (i+1)*(-1), 3);

    GenerateBox();
    SteppedFall(75);
    FillEmptyBox(25);

    KeepItClean(25);

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

    FloodAll();

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
                KeepItClean(0);
                Score = 0;
                GenerateBox();
                break;        
            case 'b':
                FillItBlue(SelectionCursorX, SelectionCursorY);
                break;
            case 'q':
                REP(i, 5) ExhaustiveFlood(i+1, (i+1)*(-1), 3);
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

    OuterBorder();
    MainMenu();
    HighScore();

    GenerateCleanBox();
    GenerateBox();

    Movement();

    if(getch()){
        GenerateBox();
    }
}