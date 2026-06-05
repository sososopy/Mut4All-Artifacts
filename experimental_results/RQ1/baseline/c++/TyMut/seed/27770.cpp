
struct T {
    template <int x>
    int get(int v){return v;}

    template <int x>
    T* getT(){return this;}

    template <int x>
    int get2(){return getT<x>()->get<x>(5);}
};

int main() {
    return T().get2<3>();
}
