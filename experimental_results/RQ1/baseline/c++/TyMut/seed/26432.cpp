
friend constexpr auto tag_invoke(custom::to_char, derived_type const a) noexcept
    requires(requires { { a.to_char() }; })
{
    return a.to_char();
}
