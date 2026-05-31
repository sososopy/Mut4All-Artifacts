use proc_macro2::{Span, *};
use quote::*;
use syn::{
    ImplItem, Item, Lifetime, LifetimeParam, parse_quote, punctuated::Punctuated,
    token, visit_mut::VisitMut, Generics, Ident, ItemImpl, Type
};
use crate::mutator::Mutator;

pub struct Introduce_Lifetime_Annotations_To_Associated_Types_38;

impl Mutator for Introduce_Lifetime_Annotations_To_Associated_Types_38 {
    fn name(&self) -> &str {
        "Introduce_Lifetime_Annotations_To_Associated_Types_38"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                let mut generics = item_impl.generics.clone();
                let mut added_lifetime = false;

                for impl_item in &mut item_impl.items {
                    if let ImplItem::Type(assoc_type) = impl_item {
                        if let Type::Path(type_path) = &assoc_type.ty {
                            let lifetime_name = Ident::new("b", Span::call_site());
                            let new_lifetime_param: LifetimeParam = parse_quote! { 'b };
                            
                            if !added_lifetime {
                                generics.params.push(parse_quote! { 'b });
                                added_lifetime = true;
                            }

                            let mut new_path = type_path.clone();
                            if let Some(last_segment) = new_path.path.segments.last_mut() {
                                last_segment.arguments = syn::PathArguments::AngleBracketed(parse_quote! { <'b> });
                            }
                            
                            assoc_type.ty = Type::Path(new_path);
                        }
                    }
                }

                if added_lifetime {
                    item_impl.generics = generics;
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets associated types within trait implementations, adding a new lifetime annotation to them. It stresses the lifetime resolution mechanism of the Rust compiler by introducing explicit lifetimes that may not align with original expectations, potentially exposing compiler bugs related to region constraints and lifetime bounds."
    }
}