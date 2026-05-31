#[macro_export]
macro_rules! token_weight {
    [;]         => {1.1};
    ($t: tt)    => {0.5};
}

#[macro_export]
macro_rules! gen_token {
    ($t: tt) => {
        syn::Token![$t](proc_macro2::Span::call_site())
    };
}

#[macro_export]
macro_rules! rnd_token {
    ($t: tt) => {
        if $crate::util::glob_range(0. .. 1.) < $crate::token_weight![$t] {
            Some($crate::gen_token!($t))
        } else {
            None
        }
    };
}
