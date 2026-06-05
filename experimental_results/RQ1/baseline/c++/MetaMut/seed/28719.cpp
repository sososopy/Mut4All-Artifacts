
module;

typedef struct {} __mbstate_t;
template<typename _StateT> class fpos;

export module hello:format;
export void format (const fpos<__mbstate_t> &);

