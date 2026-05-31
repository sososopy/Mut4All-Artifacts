use syn::{parse_quote, Type};
use once_cell::sync::Lazy;

static types: Lazy<Vec<Type>> = Lazy::new(|| {
    vec![
        parse_quote!(i32),
        parse_quote!(String),
        parse_quote!(Vec<i32>),
    ]
});