
    struct myStruct
    {
        std::vector<std::unique_ptr<ItemBase>> items;
        ...
    
    // This compiles with g++ 4.8.2 and clang++ 3.4
    inline const std::vector<std::unique_ptr<ItemBase>>& getItems() const	{ return category->getItems(); }
 
    // This DOES NOT compile with g++ 4.8.2, but compiles with clang++ 3.4
    inline const decltype(items)& getItems() const	{ return category->getItems(); }

    }
