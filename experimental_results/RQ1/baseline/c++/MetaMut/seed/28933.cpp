
// m3.cc
export module m3;
export namespace A{
}

// m2.cc
export module m2;
import m3;
export namespace A::B{
}

// m1.cc
export module m1;
import m2;
export namespace A::B{
}

