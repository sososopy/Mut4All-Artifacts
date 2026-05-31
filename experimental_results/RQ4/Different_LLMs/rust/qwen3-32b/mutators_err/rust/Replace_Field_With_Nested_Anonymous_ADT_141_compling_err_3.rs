pub struct Replace_Field_With_Nested_Anonymous_ADT_141;

impl Mutator for Replace_Field_With_Nested_Anonymous_ADT_141 {
    fn name(&self) -> &str {
        "Replace_Field_With_Nested_Anonymous_ADT_141"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(s) = item {
                if let syn::Fields::Named(named) = &mut s.fields {
                    if named.named.len() >= 2 {
                        let idx = named.named.len() - 1;
                        let field_to_replace = &mut named.named[idx];
                        let other_field = &named.named[0];
                        let other_name = other_field.ident.as_ref().unwrap();
                        let other_type = &other_field.ty;
                        let new_type = parse_quote! {
                            struct { #other_name: #other_type }
                        };
                        field_to_replace.ty = new_type;
                    }
                }
            } else if let syn::Item::Union(u) = item {
                if let syn::Fields::Unnamed(unnamed) = &mut u.fields {
                    if unnamed.unnamed.len() >= 2 {
                        let idx = unnamed.unnamed.len() - 1;
                        let field_to_replace = &mut unnamed.unnamed[idx];
                        let other_field = &unnamed.unnamed[0];
                        let other_name = other_field.ident.as_ref().unwrap();
                        let other_type = &other_field.ty;
                        let new_type = parse_quote! {
                            union { #other_name: #other_type }
                        };
                        field_to_replace.ty = new_type;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces a field in a struct or union with an anonymous struct/union containing a field with the same name as another field in the parent type. This creates field name shadowing and forces the compiler to resolve nested ADT field names, potentially exposing bugs in field uniqueness validation and name resolution logic."
    }
}