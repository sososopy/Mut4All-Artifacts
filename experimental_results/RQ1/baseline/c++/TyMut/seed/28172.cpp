
template <const int&>
class seChildScheduleAccess {};

constexpr int DummyId = 0;

struct seGccBugJob
{
    seChildScheduleAccess<DummyId> m_test;
};

