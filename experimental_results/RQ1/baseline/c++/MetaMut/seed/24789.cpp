//$ cat lldiv_t.cpp
module;

#include "lldiv_t.h"

export module std;

export namespace std
{
using ::__gnu_cxx::lldiv_t;
}

//$ cat lldiv_t.h
#pragma once

struct lldiv_t{};

namespace __gnu_cxx
{
using ::lldiv_t;
}

namespace std
{
using ::lldiv_t;
}

