

struct header_t
{
    unsigned msg_id;
    unsigned length;
};
template<typename HEADER, unsigned MTU>
struct packet_t
{
    enum { DATA_SIZE = MTU - sizeof(HEADER) };
    packet_t() : data{}
    {
        header.msg_id = 0;
        header.length = sizeof(HEADER);
    }
    HEADER header;
    char data[DATA_SIZE];
};
struct pool
{
    pool() : packets{}
    {
    }
    packet_t<header_t, 1500> packets[1024 * 16];
};
int main()
{
    pool l;
    return sizeof(l);
}

