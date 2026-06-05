
template <class T> class Q;

template <class T> 
const Q<T> conjugate(const Q<T> &);

template<class T>
struct Q {

#define PROBLEM
#ifdef PROBLEM
        const Q conjugate() { return *this; }
#endif 

        friend const Q<T> conjugate<T>(const Q<T> &); // PROBLEM
        // friend const Q conjugate<T>(const Q &); // -- equivalent
        // friend const Q conjugate<>(const Q &);  // -- equivalent
};

template <class T> 
const Q<T> conjugate(const Q<T> &) { return Q<T>(); }

int main () {
        Q<int> q;
#ifdef PROBLEM 
        q.conjugate();
#endif
        conjugate(q);
        return 0;
}

