
    enum E { A, B };

    bool bah(const enum E a)
    {
        if (a == A)
            return false;
        if (a == B)
            return true;
    }

