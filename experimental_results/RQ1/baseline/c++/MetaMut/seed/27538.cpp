
struct Old
{
    inline constexpr Old();
    constexpr Old(std::int64_t v)
        : storage(v)
    {}
    std::int64_t storage = -1;
    static const Old magic;
};

constexpr Old::Old() { storage = 0; }

constexpr Old Old::magic = Old(12);

struct OldDerived : Old
{
    constexpr OldDerived(Old old)
        : Old(old)
    {}
    constexpr OldDerived(std::int64_t v)
        : Old(v)
    {}
    auto get() { return storage; }
    static const OldDerived magic;
};

constexpr OldDerived OldDerived::magic = OldDerived(Old::magic);

struct New : public Old
{
    New() = default;
};

template<typename Strongtypedef, typename OldType_>
struct conversions
{
    using OldType = OldType_;
    const OldType& toOld() const
    {
        return static_cast<const OldType&>(
            static_cast<const Old&>(type_safe::get(static_cast<const Strongtypedef&>(*this))));
    }
};

template<typename ST, typename OT_>
struct one_more_type : conversions<ST, OT_>
{};

struct NewDerived
    : type_safe::strong_typedef<NewDerived, New>
    , one_more_type<NewDerived, OldDerived>
{
    NewDerived() = default;
    NewDerived(std::int64_t value)
        : strong_typedef(New{{value}})
    {}
};

//As a colleague already pointed out, conversions::toOld() might have
//an illegal cast in it and maybe that is all to this. What I observe is the following code prints 'VALUE IS 0' (in production: VALUE IS <GARBAGE>).

OldDerived
callee(bool b)
{
    NewDerived a = NewDerived{11};
    return b ? a.toOld() : OldDerived::magic;
}

__attribute_noinline__ void
caller_magic(bool b)
{
    OldDerived value = callee(b);
    std::cerr << "VALUE IS " << value.storage << std::endl;
}

void main()
{
    caller_magic(true);
}

//And if I do not use OldDerived::magic, the following code prints correctly 'VALUE IS 11'.

OldDerived
callee_lit(bool b)
{
    NewDerived a = NewDerived{11};
    return b ? a.toOld() : OldDerived(12);
}

__attribute_noinline__ void
caller_lit(bool b)
{
    OldDerived value = callee_lit(b);
    std::cerr << "VALUE IS " << value.storage << std::endl;
}

void main()
{
    caller_lit(true);
}

