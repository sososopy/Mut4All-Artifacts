

    struct Point { double x, y; };

    constexpr double xscale = 10;
    constexpr double yscale = 0.8;

    constexpr Point scale(Point p) { return 
{xscale*p.x,yscale*p.y};}

    int main() {
        Point p2 {10,10};
        constexpr Point p6 = scale(p2); // Error 'p2 not usable in constant expression'
    }

