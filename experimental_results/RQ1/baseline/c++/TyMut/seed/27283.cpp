
template <typename type>
static auto yolo(type) {}

auto main(void) -> int {
    &yolo<int>; // this line added
    yolo(&yolo<int>);
}
