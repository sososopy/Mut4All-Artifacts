

	template <typename T>
	struct outer
	{
		template <typename U>
		struct inner
		{

		};
	};


	template <typename T>
	struct is_inner_for
	{
		template <typename Whatever>
		struct predicate
		{
			static constexpr bool value = false;
		};

		template <typename U>
		struct predicate<typename outer<T>::template inner<U>>
		{
			static constexpr bool value = true;
		};
	};

	static_assert(
		is_inner_for<int>::template predicate<
			outer<int>::inner<double>
		>::value,
		"Yay!"
	);

