

template <typename = void>
void wrapped() {
  __try {
  } __except(true) {
  }
}
template void wrapped<>();
