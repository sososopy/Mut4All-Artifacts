use crate::mutator::Mutator;
use syn::{parse_quote, Item, ReturnType, Type, TypeParamBound, Ident, Block};
use proc_macro2::Span;

pub struct Add_Nested_Impl_Trait_185;

impl Mutator for Add_Nested_Impl_Trait_185 {
    fn name(&self) -> &str {
        "Add_Nested_Impl_Trait_185"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_items = Vec::new();
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let mut has_nested_impl_trait = false;
                        for bound in &type_impl_trait.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound.path.segments.iter().any(|seg| seg.ident == "Iterator") {
                                    has_nested_impl_trait = true;
                                    break;
                                }
                            }
                        }
                        if has_nested_impl_trait {
                            let new_trait_ident = Ident::new("AdditionalTrait", Span::call_site());
                            let new_trait: Item = parse_quote! {
                                trait #new_trait_ident {
                                    fn additional_method(&self) -> i32;
                                }
                            };
                            new_items.push(new_trait);

                            let impl_trait_for_i32: Item = parse_quote! {
                                impl #new_trait_ident for i32 {
                                    fn additional_method(&self) -> i32 {
                                        *self
                                    }
                                }
                            };
                            new_items.push(impl_trait_for_i32);

                            let new_return_type: Type = parse_quote! {
                                impl Iterator<Item = impl Iterator<Item = impl #new_trait_ident + '_> + '_>
                            };
                            item_fn.sig.output = ReturnType::Type(Default::default(), Box::new(new_return_type));

                            let original_body = &item_fn.block;
                            let new_body: Block = parse_quote! {
                                {
                                    (0..2).map(move |_| (0..2).map(move |_| 42).map(|x| Box::new(x) as Box<dyn #new_trait_ident>))
                                }
                            };
                            item_fn.block = Box::new(new_body);
                        }
                    }
                }
            }
        }
        file.items.splice(0..0, new_items);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets functions with `impl Trait` in their return type, specifically focusing on nested `impl Trait` constructs. The mutation introduces an additional layer of `impl Trait` with a new trait bound. A simple auxiliary trait is declared within the function's scope, and the return type is modified to include this new trait. The function body is adjusted to ensure returned items adhere to the new trait bound."
    }
}