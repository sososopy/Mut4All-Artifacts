use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Alter_Nested_Return_Types_With_Impl_Trait_15;

impl Mutator for Alter_Nested_Return_Types_With_Impl_Trait_15 {
    fn name(&self) -> &str {
        "Alter_Nested_Return_Types_With_Impl_Trait_15"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &mut **return_type {
                        let mut new_bounds = Punctuated::new();
                        for bound in &type_impl_trait.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound.path.segments.last().unwrap().ident == "Sized" {
                                    let nested_trait_bound = TraitBound {
                                        paren_token: None,
                                        modifier: TraitBoundModifier::None,
                                        lifetimes: None,
                                        path: parse_quote!(Iterator<Item = impl Sized>),
                                    };
                                    new_bounds.push(TypeParamBound::Trait(nested_trait_bound));
                                } else {
                                    new_bounds.push(bound.clone());
                                }
                            }
                        }
                        type_impl_trait.bounds = new_bounds;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets functions with nested `impl Trait` return types. It modifies the nested trait by introducing additional layers of `impl Trait` or changing the trait being implemented. This transformation tests the compiler's handling of coercion and type inference in complex nested scenarios, potentially exposing bugs in the compiler's implementation of these features."
    }
}