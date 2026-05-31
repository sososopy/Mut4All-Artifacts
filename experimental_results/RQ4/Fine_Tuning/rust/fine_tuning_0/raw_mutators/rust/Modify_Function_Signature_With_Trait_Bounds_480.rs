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

pub struct Modify_Function_Signature_With_Trait_Bounds_480;

impl Mutator for Modify_Function_Signature_With_Trait_Bounds_480 {
    fn name(&self) -> &str {
        "Modify_Function_Signature_With_Trait_Bounds_480"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let generics = &mut func.sig.generics;
                if generics.params.is_empty() {
                    generics.params.push(parse_quote!(T: Clone));
                } else {
                    for param in &mut generics.params {
                        if let syn::GenericParam::Type(type_param) = param {
                            if type_param.bounds.is_empty() {
                                type_param.bounds.push(parse_quote!(Clone));
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies function signatures to introduce or add a `Clone` trait bound to generic type parameters. This transformation can reveal assumptions about type behavior and test the compiler's handling of trait bounds, type inference, and trait resolution."
    }
}