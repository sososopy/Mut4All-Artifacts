
template<typename T>
concept bool C() { return true; }
template bool C<int>(); // expected error: attempt to explicitly instantiate
                        // a function concept.

