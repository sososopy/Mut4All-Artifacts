
// bug_a.ii
template<typename _Tp>
struct atomic;

template<typename _Tp>
struct atomic<_Tp*>;

// bug_b.ii
import  "bug_a.ii";

template<typename _Tp>
struct atomic<_Tp*>
{
};
