
    struct Plant {
      virtual void speak() final;
    };

    void Plant::speak(){}

    struct Flower :  Plant {
      void speak(){}
    };

    int main() {}

