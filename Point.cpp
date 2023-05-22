#include "Point.h"

Point::Point()
{
    x = y = 0;
}

bool Point::operator==(Point other)
{
    if (this->x == other.x && this->y == other.y)
        return true;
    return false;
}

Point Point::operator+(const Point& p)
{
    Point result;
    result.x = this->x + p.x;
    result.y = this->y + p.y;
    return result;
}

int Point::X()
{
    return x;
}

int Point::Y()
{
    return y;
}
