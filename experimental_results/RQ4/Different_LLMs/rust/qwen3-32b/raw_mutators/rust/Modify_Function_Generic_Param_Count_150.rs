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

pub struct Modify_Function_Generic_Param_Count_150;

impl Mutator for Modify_Function_Generic_Param_Count_150 {
    fn name(&self) -> &str {
        "Modify_Function_Generic_Param_Count_150"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if !func.sig.generics.params.is_empty() {
                    func.sig.generics.params.push(parse_quote!(U));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator increases the number of generic type parameters in a function's declaration while leaving at least one call site unchanged. This creates a discrepancy between the defined and used generic signatures, potentially exposing ICE bugs in the HIR type-checking phase by causing type parameter count mismatches."
    }
}