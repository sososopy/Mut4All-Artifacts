

struct no_destr {
    no_destr() = default;
protected:
    ~no_destr() = default;
};

int main()
{
    // error: 'no_destr::~no_destr()' is protected within this context
    new no_destr ();
}
