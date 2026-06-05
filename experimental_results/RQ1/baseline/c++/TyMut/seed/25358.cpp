
template <typename Dummy>
class Test {
    struct CatDog {
        static void meow ()
        {
            CrazyHouse::TheCatDog::meow();
        }

        struct Dog {
            static void bark ();
        };
    };

    struct CrazyHouse {
        using TheCatDog = CatDog;

        static void startMadness ()
        {
            TheCatDog::meow();
            TheCatDog::Dog::bark();
        }
    };

public:
    static void init ()
    {
        CrazyHouse::startMadness();
    }
};

int main ()
{
    Test<void> t;
    t.init();
}

