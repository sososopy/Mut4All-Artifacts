# Baseline Information

This directory provides the baseline information used in RQ1. RQ1 compares Mut4All with existing compiler fuzzers on Rust and C++ compilers.

## Directory Structure

```text
baseline/
├── c++/
│   ├── CSmith/
│   ├── GrayC/
│   ├── MetaMut/
│   ├── TyMut/
│   └── YARPGen-2.0/
├── rust/
│   ├── ClozeMaster/
│   ├── Rustlantis/
│   └── RustSmith/
└── README.md
```

## Baseline Tools

| Language | Baseline Tools                                       |
| -------- | ---------------------------------------------------- |
| Rust     | ClozeMaster, Rustlantis, RustSmith                   |
| C++      | CSmith, GrayC, MetaMut, TyMut, YARPGen-2.0, Fuzz4All |

## Note on Fuzz4All

Fuzz4All is also used as an LLM-based baseline in RQ1. However, we do not include Fuzz4All artifacts in this directory because running Fuzz4All requires deploying or configuring a local large language model backend.

Users who want to reproduce the Fuzz4All baseline can obtain the tool from its official repository:

https://github.com/fuzz4all/fuzz4all

Please follow the official instructions of Fuzz4All to configure the required LLM backend and run the tool.
