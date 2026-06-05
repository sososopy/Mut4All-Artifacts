# Compiler Information

| Language | Compiler | Version / Commit |
|---|---|---|
| C++ | GCC | GCC 14.1.0 |
| C++ | Clang | Clang 18.1.0 |
| Rust | rustc | rustc 1.88 |
| Rust | gccrs | gccrs-a1a56c6 |

## Note

The full compiler source packages, build directories, and local installation directories are not included in this artifact repository.

There are several reasons:

1. The compiler source trees and build outputs are large.
3. The official repositories already provide complete source code and build instructions.

Therefore, this artifact provides the evaluated compiler versions, official source links, and retrieval/build references. Users who want to reproduce RQ1 should download or build the compiler versions listed above and configure the corresponding compiler executable paths in their local fuzzing environment.

---

# C++ Compilers

GCC and Clang.

## GCC 14.1.0

https://ftp.gnu.org/gnu/gcc/gcc-14.1.0/

## Clang 18.1.0

https://releases.llvm.org/download.html

# Rust Compilers

rustc and gccrs.

## rustc 1.88

https://github.com/rust-lang/rust/blob/main/INSTALL.md

## gccrs-a1a56c6

https://github.com/Rust-GCC/gccrs