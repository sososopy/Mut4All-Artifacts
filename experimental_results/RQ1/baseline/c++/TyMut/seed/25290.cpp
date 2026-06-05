
#define Q_OBJECT \
    __attribute__((visibility("hidden"))) static void qt_static_metacall();

namespace {
class A
{
    Q_OBJECT
};
}

