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

pub struct Modify_Function_Generics_And_Traits_407;

impl Mutator for Modify_Function_Generics_And_Traits_407 {
    fn name(&self) -> &str {
        "Modify_Function_Generics_And_Traits_407"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if !item_fn.sig.generics.params.is_empty() {
                    let mut modified = false;
                    for param in item_fn.sig.generics.params.iter_mut() {
                        if let GenericParam::Type(type_param) = param {
                            if type_param.bounds.is_empty() {
                                let trait_bound: TypeParamBound = parse_quote!(for<U> Fn(U));
                                type_param.bounds.push(trait_bound);
                                modified = true;
                            }
                        }
                    }
                    if modified {
                        let closure_trait_bound: TypeParamBound = parse_quote!(for<U> Fn(U));
                        item_fn.sig.generics.make_where_clause().predicates.push(parse_quote!(for<U> T: #closure_trait_bound));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies functions with generics and modifies their trait bounds by adding a non-lifetime binder with a `for<>` clause. This introduces a new trait constraint requiring the generic type to implement a function-like trait over arbitrary types. The purpose is to expose potential issues related to trait resolution and type inference in the compiler."
    }
}