
constexpr bool all_zeros()
{
    union mix {
        double numeric;
        char bytes[sizeof(double)+1];
    };
    mix zero{-0.0}; // active is 'numeric'

    // setting a different active member(?):
    zero.bytes[sizeof(double)] = '\0';
    for (unsigned i=0 ; i != sizeof(double) ; ++i)
    {
        // or this is illegal (since it was never initialized through 'bytes')
        if (zero.bytes[i])
           return false;
    }
    return true;
}



int main()
{
  //  return all_zeros();
    return std::integral_constant<bool, all_zeros()>::value;
}

