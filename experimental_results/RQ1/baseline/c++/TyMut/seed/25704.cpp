
struct __declspec(uuid("{12345678-1234-1234-1234-1234567890aB}")) uuid;

template <typename T>
struct thing {
};

const struct _GUID *x = &__uuidof(thing<uuid>);
