use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Modify_Return_Type_Of_ImplTrait_Functions_162;

impl Mutator for Modify_Return_Type_Of_ImplTrait_Functions_162 {
    fn name(&self) -> &str {
        "Modify_Return_Type_Of_ImplTrait_Functions_162"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let mut new_bounds = Punctuated::new();
                        for bound in &type_impl_trait.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                let new_trait_bound: TraitBound = parse_quote!(impl Clone);
                                new_bounds.push(TypeParamBound::Trait(new_trait_bound));
                            } else {
                                new_bounds.push(bound.clone());
                            }
                        }
                        let new_return_type = Type::ImplTrait(TypeImplTrait {
                            impl_token: type_impl_trait.impl_token,
                            bounds: new_bounds,
                        });
                        item_fn.sig.output =
                            ReturnType::Type(Default::default(), Box::new(new_return_type));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets functions with `impl Trait` return types and replaces the trait with `impl Clone`. This transformation forces a change in the return type's trait constraints, which may require adjustments in the function body to return a type that implements `Clone`. It tests the compiler's ability to handle changes in opaque return types and trait resolution."
    }
}