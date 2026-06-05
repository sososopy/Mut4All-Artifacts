
#include <utility>

using Function = void();
using ConstFunction = void() const;

static_assert((!std::is_convertible<ConstFunction, Function>::value), "");
static_assert((!std::is_convertible<ConstFunction, Function*>::value), ""); // convertible
static_assert((!std::is_convertible<ConstFunction, Function&>::value), ""); // convertible
static_assert((!std::is_convertible<ConstFunction, Function&&>::value), ""); // convertible
static_assert((!std::is_convertible<Function*, ConstFunction>::value), "");
static_assert((!std::is_convertible<Function&, ConstFunction>::value), "");
static_assert((!std::is_convertible<ConstFunction, ConstFunction>::value), "");
static_assert((!std::is_convertible<ConstFunction, void>::value), "");
