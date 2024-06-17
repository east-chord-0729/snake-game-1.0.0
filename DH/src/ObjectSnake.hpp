#pragma once

#include "Object.hpp"
#include <deque>

class SnakeSegment : public Object
{
  public:
    SnakeSegment(int y, int x);
};

SnakeSegment::SnakeSegment(int y, int x)
{
    this->y = y;
    this->x = x;
    this->icon = '#';
}

class Snake
{
  public:
    Snake(int x = 0, int y = 0, Direction d = NONE);
    void addHead(SnakeSegment head);
    void addTail(SnakeSegment tail);
    void removeHead();
    void removeTail();
    Direction getDirection();
    void setDirection(Direction d);
    SnakeSegment getHead();
    SnakeSegment getTail();
    SnakeSegment getNextHead();
    int getLength(); // return the length of the snake

  private:
    std::deque<SnakeSegment> segments;
    Direction direction;
};

Snake::Snake(int x, int y, Direction d)
{
    addHead(SnakeSegment(x, y));
    setDirection(d);
}

void Snake::addHead(SnakeSegment segment)
{
    this->segments.push_front(segment);
}

void Snake::addTail(SnakeSegment segment)
{
    this->segments.push_back(segment);
}

void Snake::removeHead()
{
    this->segments.pop_front();
}

void Snake::removeTail()
{
    this->segments.pop_back();
}

Direction Snake::getDirection()
{
    return this->direction;
}

void Snake::setDirection(Direction d)
{
    direction = d;
}

SnakeSegment Snake::getHead()
{
    return segments.front();
}

SnakeSegment Snake::getTail()
{
    return segments.back();
}

SnakeSegment Snake::getNextHead()
{
    int nextY, nextX;

    nextY = this->getHead().getY();
    nextX = this->getHead().getX();

    switch (direction)
    {
    case UP:
        nextY--;
        break;
    case DOWN:
        nextY++;
        break;
    case LEFT:
        nextX--;
        break;
    case RIGHT:
        nextX++;
        break;
    default:
        break;
    }

    return SnakeSegment(nextY, nextX);
}

int Snake::getLength()
{
    return segments.size();
}