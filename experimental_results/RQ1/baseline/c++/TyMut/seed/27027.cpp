

template <typename T>
struct vector
{};

template <typename T>
struct z
{};

template <>
template <> // extra
struct z<vector<int> >
{};

