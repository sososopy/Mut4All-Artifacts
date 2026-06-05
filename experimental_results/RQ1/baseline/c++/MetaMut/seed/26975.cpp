

namespace std {

template <class _Tp> struct  remove_reference {typedef _Tp type;};
template <class _Tp> struct  remove_reference<_Tp&> {typedef _Tp type;};
template <class _Tp> struct  remove_reference<_Tp&&> {typedef _Tp type;};

template <class _Tp>
inline
_Tp&&
forward(typename std::remove_reference<_Tp>::type& __t) throw()
{
    return static_cast<_Tp&&>(__t);
}

template <class _Tp>
inline
_Tp&&
forward(typename std::remove_reference<_Tp>::type&& __t) throw()
{
    return static_cast<_Tp&&>(__t);
}

template<class _Fp> class function;

namespace __function
{

template<class _Fp> class __base;

template<class _Rp, class ..._ArgTypes>
class __base<_Rp(_ArgTypes...)>
{
public:
    virtual _Rp operator()(_ArgTypes&& ...) = 0;
};

template<class _FD, class _Alloc, class _FB> class __func;

template<class _Fp, class _Alloc, class _Rp, class ..._ArgTypes>
class __func<_Fp, _Alloc, _Rp(_ArgTypes...)>
    : public __base<_Rp(_ArgTypes...)>
{
public:
    virtual _Rp operator()(_ArgTypes&& ... __arg);
};



template<class _Fp, class _Alloc, class _Rp, class ..._ArgTypes>
_Rp
__func<_Fp, _Alloc, _Rp(_ArgTypes...)>::operator()(_ArgTypes&& ... __arg)
{
    return {};
}
}

template<class _Rp, class ..._ArgTypes>
class function<_Rp(_ArgTypes...)>
{
    typedef __function::__base<_Rp(_ArgTypes...)> __base;
    __base* __f_;

public:

    template<class _Fp>
      function(_Fp);

    _Rp operator()(_ArgTypes...) const;
};


template<class _Rp, class ..._ArgTypes>
template <class _Fp>
function<_Rp(_ArgTypes...)>::function(_Fp)
    : __f_(0)
{
}


template<class _Rp, class ..._ArgTypes>
_Rp
function<_Rp(_ArgTypes...)>::operator()(_ArgTypes... __arg) const
{
    return (*__f_)(std::forward<_ArgTypes>(__arg)...);
}

}

void setVisible()
{
  struct Visiting { };

  std::function<void(Visiting)> VisitModule { [&](Visiting) { } };
  VisitModule({});
}


