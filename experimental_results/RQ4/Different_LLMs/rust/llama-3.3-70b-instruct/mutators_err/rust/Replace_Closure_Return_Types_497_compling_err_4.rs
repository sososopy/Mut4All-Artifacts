use syn::{parse_quote, Item, ImplItem, ReturnType, Type, TypePath};
use std::collections::HashSet;
use rand::thread_rng;
use crate::mutator::Mutator;

struct Replace_Closure_Return_Types_497;

impl Mutator for Replace_Closure_Return_Types_497 {
    fn name(&self) -> &str {
        "Replace_Closure_Return_Types_497"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut existing_types = HashSet::new();

        // Collect existing types in the file
        for item in &file.items {
            if let Item::Struct(struct_item) = item {
                existing_types.insert(struct_item.ident.clone());
            }
            if let Item::Enum(enum_item) = item {
                existing_types.insert(enum_item.ident.clone());
            }
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let ReturnType::Type(_, ty) = &mut func.sig.output {
                    if let Type::Path(TypePath { qself: None, path }) = ty {
                        let replacement_type = existing_types.iter().choose(&mut thread_rng()).unwrap();
                        *ty = parse_quote! { #replacement_type };
                    }
                }
            }
            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(func) = impl_item {
                        if let ReturnType::Type(_, ty) = &mut func.sig.output {
                            if let Type::Path(TypePath { qself: None, path }) = ty {
                                let replacement_type = existing_types.iter().choose(&mut thread_rng()).unwrap();
                                *ty = parse_quote! { #replacement_type };
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the return types of closures with existing types in the program. This transformation aims to test the compiler's ability to handle closures with different return types, potentially triggering type inference and checking issues."
    }
}