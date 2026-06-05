

export module Expr;

template <typename T> class TBase {};

template<template <typename> typename TTpl>
class TBase<TTpl<int>>
{
public:
  template<template <typename> typename UTpl>
  void Foo (TBase<UTpl<int>> &rhs);
};

void Frob (TBase<int>&);
