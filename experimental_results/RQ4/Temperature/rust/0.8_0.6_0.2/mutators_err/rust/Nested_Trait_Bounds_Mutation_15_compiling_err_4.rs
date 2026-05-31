use proc_macro2::{Span, *};
use quote::*;
use syn::{
    File, Item, ItemFn, ReturnType, Type, TypeImplTrait, TypeParamBound, TraitBound, 
    punctuated::Punctuated, spanned::Spanned, token, Ident, PathSegment, PathArguments,
    parse_quote, *,
};

use crate::mutator::Mutator;

pub struct Nested_Trait_Bounds_Mutation_15;

impl Mutator for Nested_Trait_Bounds_Mutation_15 {
    fn name(&self) -> &str {
        "Nested_Trait_Bounds_Mutation_15"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &mut **return_type {
                        let mut nested_trait_bounds = Vec::new();
                        for bound in &mut type_impl_trait.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if let Some(nested_trait_bound) = find_nested_trait_bound(trait_bound) {
                                    nested_trait_bounds.push((trait_bound, nested_trait_bound));
                                }
                            }
                        }
                        for (trait_bound, nested_trait_bound) in nested_trait_bounds {
                            replace_or_nest_trait(&mut trait_bound.path.segments, nested_trait_bound);
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets functions returning `impl Trait` with nested `impl Trait` structures. It identifies nested trait bounds and either replaces the inner trait with another compatible trait or adds an additional layer of `impl Trait` nesting. This transformation is intended to stress test the compiler's handling of nested trait bounds and opaque types, potentially uncovering issues in trait resolution or type inference."
    }
}

fn find_nested_trait_bound<'a>(trait_bound: &'a TraitBound) -> Option<&'a TraitBound> {
    for segment in &trait_bound.path.segments {
        if let PathArguments::AngleBracketed(angle_bracketed) = &segment.arguments {
            for arg in &angle_bracketed.args {
                if let GenericArgument::Type(Type::ImplTrait(type_impl_trait)) = arg {
                    for bound in &type_impl_trait.bounds {
                        if let TypeParamBound::Trait(nested_trait_bound) = bound {
                            return Some(nested_trait_bound);
                        }
                    }
                }
            }
        }
    }
    None
}

fn replace_or_nest_trait(segments: &mut Punctuated<PathSegment, token::PathSep>, nested_trait_bound: &TraitBound) {
    let new_trait: TraitBound = parse_quote!(Clone);
    segments.push(PathSegment {
        ident: Ident::new("Item", Span::call_site()),
        arguments: PathArguments::AngleBracketed(parse_quote!(<impl #new_trait>)),
    });
}