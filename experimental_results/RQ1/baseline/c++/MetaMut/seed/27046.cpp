

void f(int&){}

int main()
{
    int x = 0;
    double y = 1;
    f(1 > 0 ? x : y);
}

