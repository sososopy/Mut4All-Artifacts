
typedef const char *const&  ProtocolIdType;
//typedef int ProtocolIdType;

template <ProtocolIdType protocolId>
class C {
public:
    typedef int ProtocolVersion;

    class D
    {
    public:
        ProtocolVersion GetProtocolVersion();
    };

};
template <ProtocolIdType protocolId>
typename C<protocolId>::ProtocolVersion C<protocolId>::D::GetProtocolVersion()
{
    return 1;
}

int main(void)
{
}
