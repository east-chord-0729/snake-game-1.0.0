#include "snake.h" // Snake 게임 관련 함수 및 구조체들이 정의된 "snake.h" 헤더 파일을 포함합니다.

SnakeGame::SnakeGame(int height, int width) // SnakeGame 클래스의 생성자 정의
{
    board = Board(height, width); // 보드 객체를 생성하고 설정합니다.
    initialize();                 // 초기화 함수를 호출하여 게임을 초기화합니다.
}

SnakeGame::~SnakeGame() // SnakeGame 클래스의 소멸자 정의
{
    if (apple != nullptr) // 만약 사과가 존재한다면
    {
        destroyApple(); // 사과를 파괴합니다.
    }
}

void SnakeGame::initialize() // 게임 초기화 함수 정의
{
    apple = nullptr;    // 사과 포인터를 초기화합니다.
    board.initialize(); // 보드를 초기화합니다.
    gameOver = false;   // 게임 종료 플래그를 거짓으로 설정합니다.
    srand(time(NULL));  // 난수 생성기를 초기화합니다.

    // 초기 뱀의 방향을 아래쪽으로 설정합니다.
    snake.setDirection(DOWN);

    // 초기 뱀의 위치 설정
    handleNextPiece(SnakePiece(1, 1)); // 머리
    handleNextPiece(snake.nextHead()); // 몸통
    handleNextPiece(snake.nextHead()); // 몸통
    snake.setDirection(RIGHT);         // 초기 뱀의 방향을 오른쪽으로 설정합니다.
    handleNextPiece(snake.nextHead()); // 몸통

    // 초기 사과 생성
    if (apple == nullptr)
    {
        createApple(); // 사과를 생성합니다.
    }
}

void SnakeGame::processInput() // 사용자 입력 처리 함수 정의
{
    // timeout(0); //! 입력이 없으면 바로 리턴하도록 설정

    chtype input = board.getInput(); // 보드에서 입력을 받습니다.
    //! int input = getch();

    switch (input)
    {
    case KEY_UP:                // 위쪽 화살표 키 입력
        snake.setDirection(UP); // 뱀의 방향을 위쪽으로 설정합니다.
        break;
    case KEY_DOWN:                // 아래쪽 화살표 키 입력
        snake.setDirection(DOWN); // 뱀의 방향을 아래쪽으로 설정합니다.
        break;
    case KEY_LEFT:                // 왼쪽 화살표 키 입력
        snake.setDirection(LEFT); // 뱀의 방향을 왼쪽으로 설정합니다.
        break;
    case KEY_RIGHT:                // 오른쪽 화살표 키 입력
        snake.setDirection(RIGHT); // 뱀의 방향을 오른쪽으로 설정합니다.
        break;
    case 'p':                 // 'p' 키 입력 (일시 정지)
        board.setTimeout(-1); // 시간 초과 설정을 무한대로 변경하여 일시 정지 상태로 전환합니다.
        while (board.getInput() != 'p') // 'p' 키를 누를 때까지 대기합니다.
            ;
        board.setTimeout(1000); // 시간 초과 설정을 원래대로 돌려놓습니다.
        break;
    default:
        break;
    }
}

void SnakeGame::updateState() // 게임 상태 업데이트 함수 정의
{
    // static int timer = 0;         // 타이머 초기화
    // static int moveInterval = 50; // 0.05초마다 이동

    // if (++timer >= moveInterval)
    // {
    //     timer = 0;
    //     handleNextPiece(snake.nextHead()); // 다음 뱀의 위치를 처리합니다.
    // }

    handleNextPiece(snake.nextHead()); // 다음 뱀의 위치를 처리합니다.

    // 사과가 없는 경우에는 새로운 사과를 생성합니다.
    if (apple == nullptr)
    {
        createApple(); // 새로운 사과를 생성합니다.
    }
}

void SnakeGame::redraw() // 화면 다시 그리기 함수 정의
{
    board.refresh(); // 보드를 다시 그려 화면을 새로 고칩니다.
}

bool SnakeGame::isOver() // 게임 종료 여부 확인 함수 정의
{
    return gameOver; // 게임 종료 상태를 반환합니다.
}

void SnakeGame::handleNextPiece(SnakePiece next) // 다음 위치 처리 함수 정의
{
    // 다음 위치에 사과가 없으면
    if (apple != nullptr && (next.getX() != apple->getX() || next.getY() != apple->getY()))
    {
        int emptyRow = snake.tail().getY();   // 뱀의 꼬리의 행 좌표를 가져옵니다.
        int emptyCol = snake.tail().getX();   // 뱀의 꼬리의 열 좌표를 가져옵니다.
        board.add(Empty(emptyRow, emptyCol)); // 빈 공간을 추가합니다.
        snake.removePiece();                  // 뱀의 꼬리를 제거합니다.
    }
    else // 다음 위치에 사과가 있으면
    {
        destroyApple(); // 사과를 제거합니다.
    }

    board.add(next);      // 다음 위치를 보드에 추가합니다.
    snake.addPiece(next); // 다음 위치를 뱀에 추가합니다.
}

void SnakeGame::createApple() // 사과 생성 함수 정의
{
    int y, x;
    board.getEmptyCoordinates(y, x); // 빈 공간의 좌표를 가져옵니다.
    apple = new Apple(y, x);         // 새로운 사과를 생성합니다.
    board.add(*apple);               // 사과를 보드에 추가합니다.
}

void SnakeGame::destroyApple() // 사과 제거 함수 정의
{
    delete apple;    // 사과를 삭제합니다.
    apple = nullptr; // 사과 포인터를 nullptr로 설정합니다.
}
