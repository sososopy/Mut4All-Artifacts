
class PropertyBase {};
class Number : PropertyBase {};

class ComplexProperty : PropertyBase {
public:
    Number m_firstNumber;
    static PropertyBase ComplexProperty::* const m_members[];

    // compiles fine
    static constexpr auto xxx = (Number ComplexProperty::*)&ComplexProperty::m_firstNumber;

    //  error: '(PropertyBase ComplexProperty::*)&ComplexProperty::m_firstNumber' is not a constant expression
    static constexpr auto yyy = (PropertyBase ComplexProperty::*)&ComplexProperty::m_firstNumber;
};

// compiles fine
PropertyBase ComplexProperty::* const ComplexProperty::m_members[] = {
        (PropertyBase ComplexProperty::*) &ComplexProperty::m_firstNumber
};

static ComplexProperty person;

int main() {

}
