
class DB {
        protected:
        void foo() {};
};
class DC : public DB {
        public:
        DC() {
                [this]() {
                        DB::foo(); // #1
                }();
        };
};

int main(void)
{
        DC x;
}
