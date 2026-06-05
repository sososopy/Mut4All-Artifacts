bool handle_error();

constexpr int echo(int value, bool yes = true) noexcept
{
    return (yes || handle_error()), value;
}

static_assert(echo(10) == 10, "");

