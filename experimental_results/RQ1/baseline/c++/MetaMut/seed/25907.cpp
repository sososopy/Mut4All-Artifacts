
template <std::size_t O, class C, std::size_t ...I>
inline C get_tuple_arg(lua_State* const L, indices<I...> const)
{
  C result;

  [](...){}((
    lua_rawgeti(L, O, I + 1),
    std::get<I>(result) = get_arg<-1,
      typename std::tuple_element<I, C>::type>(L),
    lua_pop(L, 1),
    0)...);
  
  return result;
}
