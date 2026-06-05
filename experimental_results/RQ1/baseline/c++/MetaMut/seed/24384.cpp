

template <typename INT,INT P> class Zp;

template <typename INT,INT P> 
Zp<INT,P> operator-(const Zp<INT,P>& a, const Zp<INT,P>& b);


template <typename INT,INT P>
class Zp {
public:
  static const INT p = P;
private:


  INT val;

public:

  Zp() : val(0) {}
  Zp( INT x ) : val(x%p) { if (x < 0 ) x+= p; }

// this compiles only if the following definition is moved AFTER the friend declataion
  Zp  operator-() const { return Zp(p-val); }
  friend Zp<INT,P> operator- <>(const Zp<INT,P>& a, const Zp<INT,P>& b);
/*
Quest_Templ.C:23:28: error: declaration of 'operator-' as non-function
   friend Zp<INT,P> operator- <Zp<>(const Zp<INT,P>& a, const Zp<INT,P>& b);
                            ^
Quest_Templ.C:23:28: error: expected ';' at end of member declaration
Quest_Templ.C:23:30: error: expected unqualified-id before '<' token
   friend Zp<INT,P> operator- <Zp<>(const Zp<INT,P>& a, const Zp<INT,P>& b);

*/
};
