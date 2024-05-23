#include "snake.h" // Snake 게임 관련 함수 및 구조체들이 정의된 "snake.h" 헤더 파일을 포함합니다.

Board::Board() // Board 클래스의 기본 생성자 정의
{
    construct(0, 0); // construct 함수 호출하여 보드 생성
}

Board::Board(int height, int width) // Board 클래스의 생성자 정의
{
    construct(height, width); // construct 함수 호출하여 보드 생성
}

//* 아마 너가 만져야 할 곳. 벽을 만들어야 할 수도 있다.
void Board::initialize() // 보드 초기화 함수 정의
{
    //! Wall, WallHard 클래스를 이용해서 맵을 만듬. 장애물 포함.
    //! 스네이크가 벽에 부딪히면, 게임 오버까지 구현. -> SnakeGame.cpp에 구현해야 할수도 있음.
    //! 따라서 소스파일을 전부 JC 폴더에 옮겨서 구현.
    clear();   // 보드를 지우고
    refresh(); // 화면을 새로 고칩니다.
}

void Board::addBorder() // 테두리 추가 함수 정의
{
    box(boardWin, 0, 0); // 지정된 윈도우에 상자 모양의 테두리를 추가합니다.
}

void Board::add(Drawable drawable) // Drawable 객체 추가 함수 정의
{
    addAt(drawable.getY(), drawable.getX(), drawable.getIcon()); // 특정 위치에 Drawable 객체를 추가합니다.
}

void Board::addAt(int y, int x, chtype ch) // 특정 위치에 문자 추가 함수 정의
{
    mvwaddch(boardWin, y, x, ch); // 지정된 윈도우의 특정 위치에 지정된 문자를 추가합니다.
}

chtype Board::getInput() // 입력 받기 함수 정의
{
    return wgetch(boardWin); // 지정된 윈도우에서 입력을 받아 반환합니다.
}

void Board::getEmptyCoordinates(int &y, int &x) // 빈 좌표 가져오기 함수 정의
{
    // 보드에서 빈 공간의 좌표를 가져옵니다.
    while ((mvwinch(boardWin, y = rand() % height, x = rand() % width)) != ' ')
        ;
}

chtype Board::getCharAt(int y, int x) // 특정 위치 문자 가져오기 함수 정의
{
    return mvwinch(boardWin, y, x); // 지정된 위치의 문자를 가져와 반환합니다.
}

void Board::clear() // 보드 지우기 함수 정의
{
    wclear(boardWin); // 지정된 윈도우를 지웁니다.
    addBorder();      // 테두리를 다시 추가합니다.
}

void Board::refresh() // 화면 새로고침 함수 정의
{
    wrefresh(boardWin); // 지정된 윈도우를 새로 고칩니다.
}

void Board::setTimeout(int timeout) // 입력 대기 시간 설정 함수 정의
{
    wtimeout(boardWin, timeout); // 지정된 윈도우에서 입력 대기 시간을 설정합니다.
}

void Board::construct(int height, int width) // 보드 생성 함수 정의
{
    int xMax, yMax;
    getmaxyx(stdscr, yMax, xMax); // 현재 윈도우의 크기를 가져옵니다.

    this->height = height; // 보드의 높이를 설정합니다.
    this->width = width;   // 보드의 너비를 설정합니다.

    // 새로운 윈도우를 생성하여 보드 윈도우를 설정합니다.
    boardWin = newwin(height, width, (yMax / 2) - (height / 2), (xMax / 2) - (width / 2));
    wtimeout(boardWin, 1000); // 입력 대기 시간을 1초로 설정합니다.
    keypad(boardWin, true);   // 키 패드 입력을 활성화합니다.
}