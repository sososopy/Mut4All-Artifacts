

#include <benchmark/benchmark.h>
#include <array>

struct A {
    A() = default;
    ~A() {
      benchmark::DoNotOptimize(c); // avoid inlining
    }
    std::array<char, 50000> member;
    char c;
};

struct B {
    B() {}  // user-defined ctor
    ~B() {
      benchmark::DoNotOptimize(c); // avoid inlining
    }
    std::array<char, 50000> member;
    char c;
};

struct C {
    // no user-defined ctor
    B b;
    int dummy;
};

// The benchmark code:

static void ACreation(benchmark::State& state) {
  for (auto _ : state) {
    A a{};
    benchmark::DoNotOptimize(a);
  }
}
BENCHMARK(ACreation);
static void BCreation(benchmark::State& state) {
  for (auto _ : state) {
    B b{};
    benchmark::DoNotOptimize(b);
  }
}
BENCHMARK(BCreation);
static void CCreation(benchmark::State& state) {
  for (auto _ : state) {
    C c{};
    benchmark::DoNotOptimize(c);
  }
}
BENCHMARK(CCreation);
BENCHMARK_MAIN();
