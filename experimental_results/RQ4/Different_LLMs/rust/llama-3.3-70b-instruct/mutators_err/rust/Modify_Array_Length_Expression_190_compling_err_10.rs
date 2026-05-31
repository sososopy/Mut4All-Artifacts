use syn::{parse_quote, Expr, Type, PathArguments, GenericArgument};

struct Modify_Array_Length_Expression_190;

impl Modify_Array_Length_Expression_190 {
    fn mutate(&self, file_ast: &mut syn::File) {
        for item in &mut file_ast.items {
            if let syn::Item::Struct(struct_item) = item {
                for field in &mut struct_item.fields {
                    if let Some(expr) = self.get_expr_from_field(&field) {
                        field.attrs.push(parse_quote!(#[allow(unused)]);
                        field.attrs.push(parse_quote!(#[allow(unused)]));
                        field.ty = parse_quote!(usize);
                    }
                }
            }
        }
    }

    fn get_expr_from_field(&self, field: &syn::Field) -> Option<Expr> {
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
}