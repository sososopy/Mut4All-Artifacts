
struct type_1
{
	static constexpr char const unique_address{};
	static constexpr void const* const id{&unique_address};
};
constexpr char const type_1::unique_address;
constexpr void const* const type_1::id;

// Another concrete type with same.
struct type_2
{
	static constexpr char const unique_address{};
	static constexpr void const* const id{&unique_address};
};
constexpr char const type_2::unique_address;
constexpr void const* const type_2::id;

// Types have different ids and these evaluate at compile time, as expected.
static_assert(type_1::id != type_2::id);

// Same, but in a template.
template <typename wrapped>
struct type_id
{
	static constexpr char const unique_address{};
	static constexpr void const* const id{&unique_address};
};

template <typename wrapped>
constexpr char const type_id<wrapped>::unique_address;

template <typename wrapped>
constexpr void const* const type_id<wrapped>::id;

// This does not compile.
static_assert(type_id<int>::id != type_id<float>::id);

