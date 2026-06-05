
#include <thread>
#include <iostream>
#include <system_error>

int main(void)
{
    std::thread t1;

    try
    {
        t1.join();
    }
    catch (const std::system_error& e)
    {
        std::cout << "System error\n";

        if (e.code() == std::errc::invalid_argument) // here in the operator==(), running into SIGSEGV 
        {
            std::cout << e.what() << "\n";
        }
        else
        {
            throw;
        }
    }

    return 0;
}
