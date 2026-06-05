

	#include <tuple>
	#include <utility>

	template <typename... Ts>
	struct my_tuple : std::tuple<Ts...>
	{
	private:
		using base_t = std::tuple<Ts...>;
	public:
		using base_t::base_t;
	};

	using std::get;

	#define TRIGGER_BUG 1
	#if TRIGGER_BUG  
	template <typename Key, typename... Ts>
	auto && get(my_tuple<Ts...>&& m)
	{
		return std::get<Key>(std::move(m));
	}

	#else
	template <typename Key, typename... Ts>
	Key && get(my_tuple<Ts...>&& m)
	{
		return std::get<Key>(std::move(m));
	}  
	#endif

	namespace detail
	{
		template <typename... Ts>
		struct inheritor
		{
		private:
			using storage_type = my_tuple<Ts...>;
			storage_type values_;

		public:
			template <typename Derived>
			struct impl : private Ts::impl...
			{
				public:
					impl(inheritor&& unsealed)
						: impl(std::move(unsealed), std::index_sequence_for<Ts...>{})
						{ }

					template <size_t... indices>
						impl(inheritor&& outer, std::index_sequence<indices...>)
						: Ts::impl(get<indices>(std::move(outer.values_)))...
						{ }
			};

			inheritor(storage_type&& values)
				: values_(std::move(values))
			{ }
		};
	}

	struct null_node
	{
		struct impl
		{
			impl(null_node&&) { }
		};
	};

	void test()
	{
		detail::inheritor<null_node> example{std::forward_as_tuple(null_node{})};
		struct whatever {};
		detail::inheritor<null_node>::impl<whatever> result(std::move(example));
	}



