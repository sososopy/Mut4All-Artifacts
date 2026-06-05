
 #include <future>

 int main()
 {
     std::packaged_task<int()> t([](){ return 1;});
     std::future<int> fut = t.get_future();
 }

