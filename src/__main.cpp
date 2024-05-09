#include "snake.h"

int main()
{
    // NCurses 초기화
    initscr();
    // 키 입력을 바로 받을 수 있도록 설정
    cbreak();
    // 입력된 키는 화면에 표시하지 않음
    noecho();

    // "Hello, NCurses!" 출력
    printw("Hello, NCurses!");
    // 출력한 내용을 실제로 화면에 반영
    refresh();

    // 사용자가 아무 키나 누를 때까지 대기
    getch();

    // NCurses 종료
    endwin();

    return 0;
}
