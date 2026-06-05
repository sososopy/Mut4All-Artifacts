
import <filesystem>;
import <iostream>;

int main() {
  std::cout << "PWD: " << std::filesystem::current_path() << std::endl;
  return 0;
}

