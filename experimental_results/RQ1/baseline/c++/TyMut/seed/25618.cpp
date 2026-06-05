namespace std __attribute__ ((__visibility__ ("default"))) {
	template<typename _Tp, typename... _Args>
	struct is_nothrow_constructible
	:	public integral_constant<bool,
		    __is_nt_constructible_helper<is_constructible<_Tp, _Args...>::value,
		    _Tp, _Args...>::__value>
	{
	};

	template<typename _Tp>
	struct default_delete
	{
	};

	template<typename _Tp>
	struct default_delete<_Tp[]>
	{
	};
}

template<typename Derived, typename Deleter = std::default_delete<const Derived> >
class refcount_base
{
	Deleter m_deleter;

	friend void
	refcnt_release(const Derived& o)
	    noexcept(
		noexcept(m_deleter(&o)) &&
		(std::is_nothrow_move_constructible<Deleter>::value ||
		 std::is_nothrow_copy_constructible<Deleter>::value) &&
		std::is_nothrow_destructible<Deleter>::value)
	{
	}
};

template<typename Type>
struct default_refcount_mgr
{
	void
	release(const Type& v) noexcept
	{
		refcnt_release(v);
	}
};

template<typename Type, typename AcqRel = default_refcount_mgr<Type> >
class refpointer
:	private AcqRel
{
public:
	typedef Type element_type;
	typedef element_type* pointer;

	~refpointer() noexcept
	{
		pointer tmp = nullptr;
		if (tmp)
			this->AcqRel::release(*tmp);
	}
};

class workq;
typedef refpointer<workq> workq_ptr;

namespace workq_detail {
	struct wq_deleter {
	};
}

class workq final
:	public workq_detail::workq_int,
	public ll_base_hook<workq_detail::runq_tag>,
	public refcount_base<workq, workq_detail::wq_deleter>
{
	class workq_pop_state
	{
	private:
		workq_ptr m_wq;
	public:
		workq_pop_state() noexcept
		:	m_wq()
		{
		}
	};
};
