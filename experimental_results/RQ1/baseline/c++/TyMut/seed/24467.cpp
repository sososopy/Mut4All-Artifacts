
struct S{
    //typedef S ThisT;
    
    template<class IdsT, class DataT>
    struct CallbackPack{
        CallbackPack(IdsT &selfIds):
            selfIds(selfIds){}

        const IdsT  &selfIds;
    };    
    
    const  CallbackPack<S, S> callbackPack123 = CallbackPack<S, S>((*this));
};
 
int main()
{
    return 0;
}

