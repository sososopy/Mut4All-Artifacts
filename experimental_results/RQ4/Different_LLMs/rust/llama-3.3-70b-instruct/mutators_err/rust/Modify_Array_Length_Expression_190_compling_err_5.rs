fn get_expr_from_field(field: &syn::Field) -> Option<Expr> {
    match &field.ty {
        Type::Array(type_array) => Some(parse_quote!(#type_array.len)),
        Type::Path(type_path) => {
            if let Some(segment) = type_path.path.segments.last() {
                if segment.ident == "Array" {
                    if let PathArguments::AngleBracketed(angle_bracketed) = &segment.arguments {
                        if let Some(arg) = angle_bracketed.args.first() {
                            if let GenericArgument::Type(Type::Array(type_array)) = arg {
                                return Some(parse_quote!(#type_array.len));
                            }
                        }
                    }
                }
            }
            None
        }
        _ => None,
    }
}