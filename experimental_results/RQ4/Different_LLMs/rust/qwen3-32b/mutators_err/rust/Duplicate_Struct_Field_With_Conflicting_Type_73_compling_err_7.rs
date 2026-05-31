use syn::parse_quote;
use syn::Type;
use rand::thread_rng;
use rand::prelude::SliceRandom;
use crate::mutator::Mutator;

pub struct Duplicate_Struct_Field_With_Conflicting_Type_73;

impl Mutator for Duplicate_Struct_Field_With_Conflicting_Type_73 {
    fn name(&self) -> &str {
        "Duplicate_Struct_Field_With_Conflicting_Type_73"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut modified_structs = Vec::new();
        let mut rng = thread_rng();

        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                if let syn::Fields::Named(named) = &mut struct_item.fields {
                    if named.named.len() >= 1 {
                        let fields: Vec<_> = named.named.iter().collect();
                        if let Some(field) = fields.choose(&mut rng) {
                            let field_name = field.ident.as_ref().unwrap().clone();
                            let new_type: Type = parse_quote!(i32);
                            let new_field = parse_quote! { #field_name: i32 };
                            named.named.push(new_field);
                            modified_structs.push((struct_item.ident.clone(), field_name, new_type));
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let syn::Item::Static(static_item) = item {
                if let syn::Type::Path(type_path) = &*static_item.ty {
                    if let Some(ident) = type_path.path.get_ident() {
                        if let Some((struct_name, field_name, _)) = modified_structs.iter().find(|(s, _, _)| s == ident) {
                            if let Some(expr_box) = static_item.expr.as_mut() {
                                if let &mut syn::Expr::Struct(ref mut expr_struct) = &mut **expr_box {
                                    let new_field = parse_quote! { #field_name = Default::default() };
                                    expr_struct.fields.push(new_field);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}