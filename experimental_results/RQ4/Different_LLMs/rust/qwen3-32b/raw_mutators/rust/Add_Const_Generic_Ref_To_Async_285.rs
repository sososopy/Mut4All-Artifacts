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

pub struct Add_Const_Generic_Ref_To_Async_285;

impl Mutator for Add_Const_Generic_Ref_To_Async_285 {
    fn name(&self) -> &str {
        "Add_Const_Generic_Ref_To_Async_285"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_none() {
                    let has_const = func.sig.generics.params.iter().any(|param| {
                        matches!(param, syn::GenericParam::Const(_))
                    });
                    if !has_const {
                        func.sig.generics.params.push(parse_quote!(const X: &'static str));
                        func.sig.asyncness = Some(syn::token::Async {
                            span: proc_macro2::Span::call_site(),
                        });
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms non-async functions into async functions and introduces a const generic parameter constrained to a reference with a static lifetime (e.g., `const X: &'static str`). This tests the compiler's handling of const generics in async contexts, potentially exposing issues in trait resolution, async trait compatibility, or const evaluation within asynchronous code."
    }
}