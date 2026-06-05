
module ;

template<typename _CharT>
  class t {
    template<typename> friend class tt;
  };
t<int> to_wstring() { }

export  module  mymodule;

