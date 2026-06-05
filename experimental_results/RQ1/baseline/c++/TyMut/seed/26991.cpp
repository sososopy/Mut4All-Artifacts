

    struct Value {
        Value();
        Value(const Value&);
        Value(Value&&);
    };

    struct Frame {
        Value value; // previously mutable
    };

    Frame top;
    const Frame& x = top;
    Value y = std::move(x.value);


