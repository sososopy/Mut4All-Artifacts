#include <cstdio>

struct Color {
    float r, g, b, a;
};

struct Test {
    Color get_value() {
        return Color{1.0f, 0.5f, 0.5f, 1.0f};
    }
    __declspec(property(get=get_value)) Color value;
};

Color get_value() {
    return Color{1.0f, 0.5f, 0.5f, 1.0f};
}
__declspec(property(get=get_value)) Color value;

int main() {
    Test t;

    auto color2 = t.value;
	printf("%f, %f, %f, %f\n", color2.r, color2.g, color2.b, color2.a);

    // returns empty ?
    auto color3 = value;
	printf("%f, %f, %f, %f\n", color3.r, color3.g, color3.b, color3.a);

    auto color4 = get_value();
	printf("%f, %f, %f, %f\n", color4.r, color4.g, color4.b, color4.a);

    return 0;
}