
friend auto operator==(list const& l, list const& r)
  noexcept(noexcept(
      std::equal(l.begin(), l.end(), r.begin(), r.end())
    )
  )
  requires(requires{std::equal(l.begin(), l.end(), r.begin(), r.end());})
{
  return std::equal(l.begin(), l.end(), r.begin(), r.end());
}

