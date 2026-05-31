use syn::{parse_quote, Type};
use rand::thread_rng;
use rand::prelude::SliceRandom;
use quote::ToTokens;
use crate::mutator::Mutator;

struct Replace_Type_Alias_With_Concrete_Type_488;

impl Mutator for Replace_Type_Alias_With_Concrete_Type_488 {
    fn name(&self) -> &str {
        "Replace_Type_Alias_With_Concrete_Type_488"
    }

    fn mutate(&self, file: &mut syn::File) {
        let file_clone = file.clone();
        for item in &mut file.items {
            if let syn::Item::Type(item_type) = item {
                if let Type::Path(type_path) = &*item_type.ty {
                    if let Some(segment) = type_path.path.segments.last() {
                        if let Some(alias) = get_type_alias(segment.ident.to_string(), &file_clone) {
                            let concrete_type = get_concrete_type(&alias);
                            item_type.ty = Box::new(concrete_type);
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces type aliases with concrete types in the seed program. It is designed to trigger bugs related to type alias impl trait. The operator applies to type aliases used in const generic parameters. To apply this mutation operator, first identify all type aliases in the seed program, then replace each type alias with a concrete type that implements the same traits as the type alias."
    }
}

fn get_type_alias(name: String, file: &syn::File) -> Option<String> {
    for item in &file.items {
        if let syn::Item::Type(item_type) = item {
            if item_type.ident.to_string() == name {
                if let Type::ImplTrait(type_impl_trait) = &*item_type.ty {
                    return Some(type_impl_trait.bounds.iter().map(|bound| bound.into_token_stream().to_string()).collect::<Vec<String>>().join(","));
                }
            }
        }
    }
    None
}

fn get_concrete_type(alias: &str) -> syn::Type {
    let mut rng = thread_rng();
    let concrete_types: Vec<syn::Type> = vec![parse_quote! { i32 }, 
                                             parse_quote! { String }, 
                                             parse_quote! { Vec<i32> }];
    let concrete_type: &syn::Type = concrete_types.choose(&mut rng).unwrap();
    concrete_type.clone()
}