
    #include <vector>
    #include <memory>

    class NonCopyable
    {
    private:
        std::vector<std::unique_ptr<int>> mElements;
    };

    int main()
    {
    	NonCopyable a;
    	NonCopyable b(a);

    	return 0;
    }

