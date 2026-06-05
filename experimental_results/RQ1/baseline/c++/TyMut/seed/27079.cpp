auto bound_f = [=](auto... xs) -> decltype(f(x, xs...))
{
    return f(x, xs...);
};

return curry_impl<decltype(bound_f),
    is_zero_callable<decltype(bound_f)>{}>::exec(bound_f);

