

struct Test : public std::pair<int, std::string> {
    using std::pair<int, std::string>::pair;
};

Test p(std::piecewise_construct, std::forward_as_tuple(1), std::forward_as_tuple("abcd"));

