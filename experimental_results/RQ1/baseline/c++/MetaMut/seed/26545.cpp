
#include <vector>
#include <string>
#include <coroutine>
#include <boost/asio.hpp>

#define REPRODUCE_ISSUE 1

boost::asio::awaitable<void>
test1(boost::asio::io_context& ioc) {
    co_await boost::asio::post(
        ioc,
        boost::asio::append(
            boost::asio::use_awaitable,
            std::vector<int> {
                1, 2, 3
            }
        )
    );
}

boost::asio::awaitable<void>
test2(boost::asio::io_context& ioc) {
    std::vector<std::string> str{ "aaa", "bbb" };
    co_await boost::asio::post(
        ioc,
        boost::asio::append(
            boost::asio::use_awaitable,
#if REPRODUCE_ISSUE
            std::vector<std::string> {
                "aaa", "bbb"
            }
#else
            str
#endif
        )
    );
}

int main() {
    boost::asio::io_context ioc;
    boost::asio::co_spawn(ioc, test1(ioc), boost::asio::detached);
    boost::asio::co_spawn(ioc, test2(ioc), boost::asio::detached);
    ioc.run();
}
