use syn::parse_quote;
use rand::thread_rng;
use rand::prelude::SliceRandom;
use crate::mutator::Mutator;

struct Modify_Inherent_Associated_Type_Definition_421;

impl Mutator for Modify_Inherent_Associated_Type_Definition_421 {
    fn name(&self) -> &str {
        "Modify_Inherent_Associated_Type_Definition_421"
    }

    fn mutate(&self, file: &mut syn::File) {
        let types = get_types(file);
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Type(item_type) = impl_item {
                        let mut rng = thread_rng();
                        let new_type_ident = types.choose(&mut rng).unwrap();
                        let new_type: syn::Type = parse_quote! { #new_type_ident };
                        item_type.ty = new_type;
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies inherent associated type definitions in the given Rust code. It replaces the type of each inherent associated type with another type that already exists in the program, potentially leading to type mismatches or inference failures. This transformation aims to test the compiler's handling of inherent associated types and their interactions with other types in the program."
    }
}

fn get_types(file: &syn::File) -> Vec<syn::Ident> {
    let mut types = Vec::new();
    for item in &file.items {
        if let syn::Item::Struct(item_struct) = item {
            types.push(item_struct.ident.clone());
        }
        if let syn::Item::Enum(item_enum) = item {
            types.push(item_enum.ident.clone());
        }
    }
    types
}