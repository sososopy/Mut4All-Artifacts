
import "iostream";
import "functional";
import "string_view";

auto test (std::string_view sv) -> void {

      std::cout << sv.substr (std::size_t {0}, std::size_t {8})
                << '\n';
}

auto main () -> int {

     test ("Free Software Community");

return 0;
}
