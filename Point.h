class Point
{
public:
    int x, y;
    Point();
    bool operator==(Point other);
    Point operator+(const Point& p);
    int X();
    int Y();
};

