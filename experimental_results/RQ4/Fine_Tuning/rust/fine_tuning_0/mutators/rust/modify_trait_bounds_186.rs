use syn::{ItemTrait, ItemImpl, Item, ItemStruct, parse_quote};
use quote::quote;
use crate::mutator::Mutator;

pub struct Modify_Trait_Bounds_186;

impl Mutator for Modify_Trait_Bounds_186 {
    fn name(&self) -> &str {
        "Modify_Trait_Bounds_186"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_c_added = false;
        let mut struct_c_added = false;
        let mut new_items = Vec::new();

        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                if !trait_c_added {
                    // Define a new trait `TraitC`
                    let trait_c: ItemTrait = parse_quote! {
                        pub trait TraitC {}
                    };
                    new_items.push(Item::Trait(trait_c));
                    trait_c_added = true;
                }

                // Modify trait bounds to include `TraitC`
                item_trait.supertraits.push(parse_quote!(TraitC));
            }
        }

        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if !struct_c_added {
                    // Define a struct `StructC` and implement `TraitC` for it
                    let struct_c: ItemStruct = parse_quote! {
                        pub struct StructC;
                    };
                    let impl_trait_c: ItemImpl = parse_quote! {
                        impl TraitC for StructC {}
                    };
                    new_items.push(Item::Struct(struct_c));
                    new_items.push(Item::Impl(impl_trait_c));
                    struct_c_added = true;
                }
            }
        }

        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                // Ensure all implementations of the original trait also implement `TraitC`
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if let Some(last_segment) = path.segments.last() {
                        if last_segment.ident == "B" {
                            let self_ty = &item_impl.self_ty;
                            new_items.push(parse_quote! {
                                impl TraitC for #self_ty {}
                            });
                        }
                    }
                }
            }
        }

        file.items.extend(new_items);
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}