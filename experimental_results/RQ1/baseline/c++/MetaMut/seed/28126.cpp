
 #include <map>
 #include <utility>
 int main() {
     std::map<std::pair<int, int>, int> m;
     m[std::pair<int, int>(0, 0)] = 0;
 }

