
void fatal [[noreturn]] (const char * msg);

template <typename ToType, typename FromType, typename MinType>
ToType boundsCheck(const FromType & value, const MinType & min)
{
    if (int(value) >= int(min))
    {
        return static_cast<ToType>(value);
    }

    fatal ("nope");
}

enum class EventCat
{
    kEventCat_NeverUseThis = 0,
    kUninitialized,
    kTesting,

    kEventCat_Min = kEventCat_NeverUseThis + 1,
};

struct Event
{
    using enum EventCat;

    Event(EventCat a_category, auto)
        : category(a_category)
    {
        boundsCheck<EventCat>(a_category, kEventCat_Min);  // using "EventCat::kEventCat_Min" instead of just "kEventCat_Min" makes this not fail
    }

    EventCat category = EventCat::kUninitialized;
};

void foo()
{
    Event(EventCat::kTesting, 0);
}
