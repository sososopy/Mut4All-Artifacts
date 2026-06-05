
    #include <variant>

    class some_class
    {
        public:
            void encode() const {}
    };

    // This template is necessary to trigger the bug.
    template <class encoder_t>
    void process() noexcept
    {
        // This variable needs to be both static and constexpr to trigger the bug.
        static constexpr some_class magic;
        // The useless visit here is necessary to trigger the bug.
        std::visit([](auto &&key) {
            // This encode function must be a member function to trigger the bug.
            magic.encode();
        }, std::variant<int>{});
    }

    // Instantiate the template above
    template void process<int>() noexcept;

