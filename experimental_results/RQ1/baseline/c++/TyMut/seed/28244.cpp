
#include <stdio.h>
#include <boost/asio.hpp>

int main(int argc, char *argv[])
{
    static const char *func = __FUNCTION__;
    boost::asio::io_service io_service;
    boost::asio::deadline_timer timer(io_service);
    timer.async_wait( [](auto) {
        printf("%s()\n", func);
    });
}

