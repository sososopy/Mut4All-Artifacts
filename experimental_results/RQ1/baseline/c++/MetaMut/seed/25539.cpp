
const int a = 1;
auto lambda = [&]() {
  &a;
};
lambda();
