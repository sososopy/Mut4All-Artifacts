use proc_macro2::{Span, *};
use quote::*;
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, token, visit_mut::VisitMut, *,
};
use crate::mutator::Mutator;

pub struct Modify_Inherent_Associated_Types_1;

impl Mutator for Modify_Inherent_Associated_Types_1 {
    fn name(&self) -> &str {
        "Modify_Inherent_Associated_Types_1"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut impl_items = Vec::new();
        
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let struct_ident = &item_struct.ident;
                let generics = &mut item_struct.generics;

                // Add a new unused type parameter to the struct
                let new_param: GenericParam = parse_quote!(U);
                generics.params.push(new_param);

                // Add PhantomData<U> to the struct fields
                if let Fields::Unnamed(fields_unnamed) = &mut item_struct.fields {
                    fields_unnamed.unnamed.push(parse_quote!(std::marker::PhantomData<U>));
                }

                // Collect impl blocks for later processing
                for impl_item in &file.items {
                    if let Item::Impl(item_impl) = impl_item {
                        if let Type::Path(TypePath { path, .. }) = &*item_impl.self_ty {
                            if path.is_ident(struct_ident) {
                                impl_items.push(item_impl.clone());
                            }
                        }
                    }
                }
            }
        }

        // Process the collected impl items
        for impl_item in impl_items {
            // Add the new type parameter to the impl block
            let mut item_impl = impl_item.clone();
            item_impl.generics.params.push(parse_quote!(U));

            // Modify the associated type to include the new type parameter
            for impl_item in &mut item_impl.items {
                if let ImplItem::Type(impl_type) = impl_item {
                    if impl_type.ident == "P" {
                        impl_type.ty = parse_quote!((T, U));
                    }
                }
            }

            // Insert the modified impl block back into the file
            file.items.push(Item::Impl(item_impl));
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds an unused type parameter `U` to structs with inherent associated types. It modifies the struct to include a `PhantomData<U>` field and updates the associated type to incorporate the new parameter. This tests the compiler's handling of type parameter substitutions and ensures that the added parameter does not affect other parts of the code."
    }
}