

    template<class S, template<typename> class V>
    void Operate(S c, const V<S>& vx);
    
    template<class T, template<typename> class U>
    class ViewBase
    {
    	template<class S, template<typename> class V>
    	friend void Operate(S c, const V<S>& vx);
    };
    
    template<class T>
    class ViewTypeA : public ViewBase<T, ViewTypeA>
    {
    };
    
    template<class T>
    class ViewTypeB : public ViewBase<T, ViewTypeB>
    {
    };
    
    template<class S, template<typename> class V>
    void Operate(S c, const V<S>& vx)
    {
    }



    
    int main(int argc, char **argv)
    {
    	Operate(5, ViewTypeA<int>());
    }

