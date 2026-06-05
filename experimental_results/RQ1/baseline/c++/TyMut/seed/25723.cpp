
#include <unordered_map>
#include <vector>
struct A
{
    typedef std::unordered_multimap<int, int> HM;
    struct B {
        B( A::HM::mapped_type & m ) : i( m ) {}
        //                   ^^ missing const
        int i;
    };  
    void foo( int const bkt, std::vector<B> intents );
    HM hmap;
};  
void A::foo( int const bkt, std::vector<A::B> intents )
{
    std::pair<HM::const_iterator,HM::const_iterator> found = hmap.equal_range( bkt );
    HM::const_iterator& it = found.first;
    for( ; it != found.second; ++it ) {
        HM::mapped_type const& mp = it->second;
        intents.emplace_back( mp ); 
    }   
}   
int main(int,char**)
{
}   
