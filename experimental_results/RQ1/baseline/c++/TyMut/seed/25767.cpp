int g_nn;
int& g_n __attribute__((deprecated)) = g_nn;

int main()
{
    g_n = 1;
}

