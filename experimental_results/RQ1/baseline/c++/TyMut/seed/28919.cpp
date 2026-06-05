
template <typename> struct Dense_Config;
template <typename... Args> using Dense = Dense_Config<Args...>;
auto layer_1 = Dense {}
