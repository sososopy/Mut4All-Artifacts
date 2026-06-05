
#pragma once

#if 1
import <string_view>;
#else
#include <string_view>
#endif

void say_hello (const std::string_view& name);

