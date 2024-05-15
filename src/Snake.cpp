#include "snake.h" // Snake 게임 관련 함수 및 구조체들이 정의된 "snake.h" 헤더 파일을 포함합니다.

SnakePiece::SnakePiece(int y, int x) // SnakePiece 클래스의 생성자 정의
{
    this->y = y; // 행 좌표를 설정합니다.
    this->x = x; // 열 좌표를 설정합니다.
    this->icon = '#'; // 뱀 조각의 아이콘을 설정합니다.
}

Snake::Snake() // Snake 클래스의 생성자 정의
{
    currDirection = DOWN; // 초기 뱀의 방향을 아래쪽으로 설정합니다.
}

void Snake::addPiece(SnakePiece piece) // 뱀 조각 추가 함수 정의
{
    prevPieces.push(piece); // 뱀의 이전 조각에 새로운 조각을 추가합니다.
}

void Snake::removePiece() // 뱀 조각 제거 함수 정의
{
    prevPieces.pop(); // 뱀의 이전 조각에서 조각을 제거합니다.
}

SnakePiece Snake::tail() // 뱀 꼬리 반환 함수 정의
{
    return prevPieces.front(); // 뱀의 이전 조각 중 가장 앞에 있는 조각을 반환합니다.
}

SnakePiece Snake::head() // 뱀 머리 반환 함수 정의
{
    return prevPieces.back(); // 뱀의 이전 조각 중 가장 뒤에 있는 조각을 반환합니다.
}

Direction Snake::getDirection() // 뱀의 현재 방향 반환 함수 정의
{
    return currDirection; // 뱀의 현재 방향을 반환합니다.
}

void Snake::setDirection(Direction d) // 뱀의 방향 설정 함수 정의
{
    // 반대 방향으로 설정되지 않도록 조건을 검사합니다.
    if (currDirection + d != 0)
    {
        currDirection = d; // 뱀의 현재 방향을 새로운 방향으로 설정합니다.
    }
}

SnakePiece Snake::nextHead() // 다음 뱀 머리 위치 반환 함수 정의
{
    int row = head().getY(); // 뱀의 머리의 행 좌표를 가져옵니다.
    int col = head().getX(); // 뱀의 머리의 열 좌표를 가져옵니다.

    // 현재 방향에 따라 다음 머리의 위치를 계산합니다.
    switch (currDirection)
    {
    case UP: // 위쪽 방향인 경우
        row--; // 행 좌표를 감소시킵니다.
        break;
    case DOWN: // 아래쪽 방향인 경우
        row++; // 행 좌표를 증가시킵니다.
        break;
    case LEFT: // 왼쪽 방향인 경우
        col--; // 열 좌표를 감소시킵니다.
        break;
    case RIGHT: // 오른쪽 방향인 경우
        col++; // 열 좌표를 증가시킵니다.
        break;
    default:
        break;
    }

    return SnakePiece(row, col); // 다음 뱀 머리의 위치를 반환합니다.
}
