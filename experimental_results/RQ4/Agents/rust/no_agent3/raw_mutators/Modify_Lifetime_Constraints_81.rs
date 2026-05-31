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

pub struct Modify_Lifetime_Constraints_81;

impl Mutator for Modify_Lifetime_Constraints_81 {
    fn name(&self) -> &str {
        "Modify_Lifetime_Constraints_81"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, ref mut return_type) = item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &mut **return_type {
                        let mut has_lifetime = false;
                        for bound in &type_impl_trait.bounds {
                            if let TypeParamBound::Lifetime(_) = bound {
                                has_lifetime = true;
                                break;
                            }
                        }
                        if has_lifetime {
                            type_impl_trait.bounds.push(TypeParamBound::Lifetime(Lifetime::new("'static", Span::call_site())));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies functions returning `impl Trait` with lifetime annotations. It modifies the lifetime constraints by adding a conflicting `'static` lifetime to the return type, potentially causing an ICE by introducing non-universal lifetime regions."
    }
}