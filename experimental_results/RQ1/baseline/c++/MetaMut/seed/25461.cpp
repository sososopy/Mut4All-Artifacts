
int main() {
    int i1 = 2, i2 = 3, i3 = 4;
    int array[i1][i2][i3];
    auto lambda = [&]() {
        int i4 = 1, i5 = 2, i6 = 3;
        int output = array[i4][i5][i6];
    };
    lambda();
}

