#include "snake.h" // Snake 게임 관련 함수 및 구조체들이 정의된 "snake.h" 헤더 파일을 포함합니다.

int main() // main 함수 시작
{
    initscr(); // ncurses 모드를 초기화합니다. 화면을 설정하고 입력 모드를 시작합니다.
    refresh(); // 화면을 새로 고칩니다. 모든 변경 사항을 화면에 반영합니다.
    noecho(); // 사용자 입력에 대한 에코를 해제합니다. 사용자 입력이 화면에 표시되지 않습니다.
    curs_set(0); // 커서를 화면에서 보이지 않도록 설정합니다.

    SnakeGame game(BOARD_ROWS, BOARD_COLS); // Snake 게임 객체를 생성합니다. 게임 보드의 행과 열을 인수로 전달합니다.

    while (!game.isOver()) // 게임이 종료되지 않은 동안 반복합니다.
    {
        game.processInput(); // 사용자 입력을 처리합니다.
        game.updateState();  // 게임 상태를 업데이트합니다. (뱀의 움직임, 충돌 체크 등)
        game.redraw(); // 화면을 다시 그립니다. (뱀과 먹이의 위치 등을 업데이트하여 화면에 표시합니다.)
    }

    getch(); // 사용자의 입력을 기다립니다. 사용자가 키를 누를 때까지 대기합니다.

    endwin(); // ncurses 모드를 종료합니다. 화면을 정리하고 입력 모드를 종료합니다.

    return 0; // 프로그램을 성공적으로 종료합니다.
}
