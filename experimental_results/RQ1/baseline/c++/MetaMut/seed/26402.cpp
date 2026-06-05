
#include <set>
#include <vector>

struct R {
    int w;
};

typedef std::vector<R> vr_t;
typedef std::set<R> sr_t;

static sr_t FilterRs (const sr_t & rs)
{
    return rs;
}

vr_t getRs ()
{
    const vr_t filteredRs = []() {
        sr_t rs;

        rs = FilterRs (rs);

        return vr_t{rs.begin(), rs.end()};
    }();

    return filteredRs;
}

