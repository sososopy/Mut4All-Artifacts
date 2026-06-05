
int foo(){
        return 2;
}

int main(){
        int (&g)() = foo;
        return 1;
}
