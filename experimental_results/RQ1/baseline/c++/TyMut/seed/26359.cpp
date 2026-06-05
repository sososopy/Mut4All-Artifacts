
import "iostream";
import "string";
import "string_view";
import "expected";
import "charconv";
import "system_error";

namespace parser {

enum class error {
      input_invalid
      , overflow
};

auto logic (std::string_view str) -> std::expected <double, error> {
      int value {0};

      std::from_chars_result result { std::from_chars (str.begin (), str.end (), value) };

      if (result.ec == std::errc{}) {
            return value;
      }
      else if (result.ec == std::errc::invalid_argument) {
            return std::unexpected {error::input_invalid};
      }
      else if (result.ec == std::errc::result_out_of_range) {
            return std::unexpected {error::overflow};
      }
}

}

auto test () -> void {
      std::string str { "9876dairy" };

      const auto result { parser::logic (str) };

      if (result.has_value ()) {
            std::cout << result.value ()
                      << '\n';
      }
      else if (result.error () == parser::error::input_invalid) {
            std::cout << "E: input invalid"
                      << '\n';
      }
      else if (result.error () == parser::error::overflow) {
            std::cout << "overflow"
                      << '\n';
      }
      else {
            std::cout << "unexpected"
                      << '\n';
      }
}

