
template<typename Type>
class Lazy {

public:
   explicit Lazy(const std::function<Type*()>&     init,
		 const std::function<void(Type*)>& fini = ([](Type *object) -> void { delete object; })
		):m_object(0),m_initializer(init),m_finalizer(fini) { }

};

Lazy<Test> lt([]{ return new Test(42); });

