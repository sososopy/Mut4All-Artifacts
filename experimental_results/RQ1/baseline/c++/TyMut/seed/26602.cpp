
struct Trans_NS___cxx11_basic_string {                                                                              
	constexpr Trans_NS___cxx11_basic_string();                                                                        
};                                                                                                                  
struct Ltt {                                                                                                        
	Trans_NS___cxx11_basic_string s;                                                                                                                                                                                        
};                                                                                                                  
template <int _Nm> struct __array_traits {                                                                          
	using _Type = Ltt[_Nm];                                                                                           
};                                                                                                                  
template <int _Nm> struct array {                                                                                   
	__array_traits<_Nm>::_Type _M_elems;                                                                              
};                                                                                                                  
template <typename EnumT> struct Map {                                                                              
	Map() noexcept = default;                                                                                         
	array<enumSize(EnumT{})> values_{};                                                                               
};                                                                                                                  
enum TechnologyType {};                                                                                             
constexpr int enumSize(TechnologyType) { return 4; }                                                                
Map<TechnologyType> cuttingLtts_;                                

