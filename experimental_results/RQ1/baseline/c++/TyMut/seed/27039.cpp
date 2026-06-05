
template<auto Signal=nullptr>
struct Prop {
    void notify()
    {
            if constexpr (Signal != nullptr) {
            }
    }
};

class QObjectPrivate {
public:
    struct ExtraData
    {
            inline void nameChangedForwarder()	{}
    };
};

int main()
{
        Prop<&QObjectPrivate::ExtraData::nameChangedForwarder>	prop;
        prop.notify();
}
