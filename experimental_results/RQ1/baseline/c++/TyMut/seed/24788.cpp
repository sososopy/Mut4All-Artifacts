struct C{
   C(int);
};
int main() {
    C *ptr{new C[]{1}}; //clang rejects but gcc accepts
}