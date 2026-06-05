

namespace carta {
    namespace lib {
	class Shape {
	public:
	    enum class Type { Circle, Square };
	};
    }
}

int main( int, char **)
{
    auto x = carta::lib::Shape::Type::Circle;
    carta::lib::Shape shape;
    carta::lib::Shape::Type y;
    y = shape.Type::Cicle;
    return 0;
}
