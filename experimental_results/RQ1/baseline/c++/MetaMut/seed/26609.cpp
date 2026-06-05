
template<class T>
template struct type_info_t{
	//...
	template<class U>
	static constexpr bool can_convert_to=XXX;
	//...
}
template<class T>
constexpr type_info_t<T>type_info{};
