
struct __declspec(uuid("{12345678-1234-1234-1234-1234567890aB}")) uuid;

template <struct _GUID *>
struct thing {
};

thing<__uuidof(uuid)> x;
thing<&__uuidof(uuid)> y;
