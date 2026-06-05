
 #include <cassert>


 template<typename T>
 class Maybe {
     bool mIsSome = false;
     T mVal = {};

 public:
     Maybe() = default;
     Maybe(const T& rhs) : mIsSome(true), mVal(rhs) {}

     T& operator*() {
         assert(mIsSome);
         return mVal;
     }
 };


 struct Int {
     int val = {};

     Int() = default;
     Int(int x) : val(x) {}
 };

 struct Even : public Int {
 public:
     static Maybe<Even> From(const Int x) {
         if (x.val & 1) return {};
         return Even{x.val};
     }

     Even() = default;
 private:
     Even(int x) : Int(x) {
         assert((val & 1) == 0);
     }
 };


 int Oops(const Int i) {
     const auto& e = *Even::From(i); // lvalue ref lifetime extension of sub-object
                                     // of temporary?
     return e.val + 9; // or UB access?
 }
