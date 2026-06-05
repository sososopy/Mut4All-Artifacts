namespace xercesc_2_5 {

    class MemoryManager;

    class XMemory
    {
    };

    template <class TElem> class XMLEnumerator
    {
    };

    template <class TElem> class NameIdPool : public XMemory
    {
    };

    template <class TElem> class NameIdPoolEnumerator : public XMLEnumerator<TElem>, public XMemory
    {
    public :
        NameIdPoolEnumerator<TElem>& operator=
                (
                        const NameIdPoolEnumerator<TElem>& toAssign
                );

    private :
        MemoryManager* const fMemoryManager;
    };

    template <class TElem> NameIdPoolEnumerator<TElem>& NameIdPoolEnumerator<TElem>::
    operator=(const NameIdPoolEnumerator<TElem>& toAssign)
    {
        fMemoryManager = toAssign.fMemoryManager;
        return *this;
    }

}