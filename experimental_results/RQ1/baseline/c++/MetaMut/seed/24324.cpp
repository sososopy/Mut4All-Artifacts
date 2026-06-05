
#include <netinet/in.h>

int main()
{
    struct sockaddr_in addr0;      // No warning. But really zero-initialized?
    //struct sockaddr_in addr1();  // No warning, but a function-declaration.
    struct sockaddr_in addr2 = {};    // In C++: Warning about all members.
                                      // In C: Warning about first member.
    struct sockaddr_in addr3 = {0};   // In C++: Warning about all members
                                      //         except the first one.
    struct sockaddr_in addr4 = {0,};  // Same here.
#ifdef __cplusplus
    struct sockaddr_in addr5 = sockaddr_in();  // No warning and (probably)
                                               // zero-initialized.
#endif
#if __cplusplus >= 201103L
    struct sockaddr_in addr6{};    // Warning about all members.
    struct sockaddr_in addr7{0};   // Warning about all members except the
                                   // first one.
    struct sockaddr_in addr8{0,};  // Same here.
    struct sockaddr_in addr9  = sockaddr_in{};    // Warning about all members.
    struct sockaddr_in addr10 = sockaddr_in{0};   // Warning about all members
                                                  // except the first one.
    struct sockaddr_in addr11 = sockaddr_in{0,};  // Same here.
#endif

    return 0;
}
