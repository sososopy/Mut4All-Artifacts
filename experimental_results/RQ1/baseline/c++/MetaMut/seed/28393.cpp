
class SortAlgorithm {
public:
    template<typename T>
    static void test(T* t, bool(*compare_func)() = 
                    []() -> bool { return true; }) {
        test(t);
    }
};

int main() {
    int arr[] { 1, 2 };
    SortAlgorithm::test(arr);
}
