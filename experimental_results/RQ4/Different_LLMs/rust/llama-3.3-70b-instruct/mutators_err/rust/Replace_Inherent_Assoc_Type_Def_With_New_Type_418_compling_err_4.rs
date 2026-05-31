use syn::{parse_quote, Type};

static types: Vec<Type> = vec![
    parse_quote!(i32),
    parse_quote!(String),
    parse_quote!(Vec<i32>),
];