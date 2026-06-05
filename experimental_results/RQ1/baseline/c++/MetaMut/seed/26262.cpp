
class C{
public:

    C(C&&) = default;

    explicit C(int (&array)[2][2]) : C({
        ._m_array = {
            {array[0][0], array[0][1]},
            {array[1][0], array[1][1]}
        }
    })
    {

    }

private:
    int _m_array[2][2];
};

int main(void){
    int array[2][2] = {
        {1, 2},
        {3, 4}
    };
    C c(array);
    return 0;
}
