#define DECLARE_INTERNAL_LINK_BASE(T) template<int Counter> struct T
#define DECLARE_INTERNAL_LINK_SPECIALIZATION(T, Counter) template<> struct T<Counter>

namespace TestBugFix
{
	struct FPlatformTypeLayoutParameters
	{
		DECLARE_INTERNAL_LINK_BASE(InternalLinkType);
		DECLARE_INTERNAL_LINK_SPECIALIZATION(InternalLinkType, __COUNTER__);
	};
}

