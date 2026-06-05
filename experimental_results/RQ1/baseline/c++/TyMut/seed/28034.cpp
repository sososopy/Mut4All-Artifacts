

#include<concepts>
#include<string>
#include<string_view>
#include<iterator>
#include<vector>
#include<deque>

int main()
{
	std::vector<std::size_t> vec;
	std::contiguous_iterator auto vec_iter(vec.cbegin());
	std::string_view sv;
	std::contiguous_iterator auto sv_iter(sv.cbegin());
	std::string str;
	std::contiguous_iterator auto str_iter(str.cbegin());
	std::deque<std::size_t> dq;
	std::random_access_iterator auto dq_iter(dq.cbegin());
	static_assert(!std::contiguous_iterator<decltype(dq_iter)>,"deque iterator should not be a contiguous_iterator");
}
