enum class SampleEnumCausingIssue {
};

struct BaseSnapshot {
    SampleEnumCausingIssue enumValue;
protected:
    ~BaseSnapshot() = default;  
};

struct Derived : BaseSnapshot {
};

int main(){
    Derived d{};
}
