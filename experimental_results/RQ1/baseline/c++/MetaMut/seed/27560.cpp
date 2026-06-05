
class Base {
public:
    virtual void vmethod();
};

class foo : public Base {
    int x;
    void vmethod() override;
};

int main() {
    foo f;
}

