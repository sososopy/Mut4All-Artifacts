
    class B { protected: void foo(void) {}; };
    class S : public B {
        public: S(void) { [this](void) { B::foo(); }(); };
    };


