
class test {
    static const test &get_instance() {
        return instance;
    }
    static thread_local test instance;
};


