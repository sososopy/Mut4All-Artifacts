
       #include <vector>

       void test()
       {
           std::vector<int> p;
           p.push_back(1);
           std::vector<int> q(p.size());
           q[0] = p[0];
       }
