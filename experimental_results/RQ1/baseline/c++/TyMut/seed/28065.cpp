
auto x = []<int = 0>(){
    return requires() {
        1;
    };
}();
