use syn::parse_quote;
use crate::mutator::Mutator;

pub struct Add_Recursive_Type_Alias_167;

impl Mutator for Add_Recursive_Type_Alias_167 {
    fn name(&self) -> &str {
        "Add_Recursive_Type_Alias_167"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_items = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let struct_name = &item_struct.ident;
                let recursive_type_alias: syn::Item = parse_quote! {
                    type RecursiveAlias = Vec<RecursiveAlias>;
                };
                new_items.push(recursive_type_alias);
                
                for field in &mut item_struct.fields {
                    if let syn::Type::Path(type_path) = &mut field.ty {
                        if type_path.path.segments.last().unwrap().ident == *struct_name {
                            type_path.path.segments.push(parse_quote!(RecursiveAlias));
                        }
                    }
                }
            }
        }
        file.items.extend(new_items);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds a recursive type alias `RecursiveAlias` that refers to itself wrapped in a `Vec`. This transformation aims to test the compiler's handling of deeply recursive types, potentially triggering infinite recursion during type resolution or causing ICEs related to stack overflow or type inference failures."
    }
}