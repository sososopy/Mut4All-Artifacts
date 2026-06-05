


enum class init_prio : int {
    first = 101,
    second,
    third,
};
__attribute__((init_priority(init_prio::second))) t a;

