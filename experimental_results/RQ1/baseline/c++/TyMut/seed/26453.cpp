
std::ifstream t("file.txt");
std::string str((std::istreambuf_iterator<char>(t)),
                 std::istreambuf_iterator<char>());
