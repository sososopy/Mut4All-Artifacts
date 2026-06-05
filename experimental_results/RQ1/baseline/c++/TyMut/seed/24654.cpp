
int main() {
    unsigned count = 5;
    bool array[count];
    [&array] () {
      array[0] = sizeof(array) > 5;
    }();
    return 0;
}

