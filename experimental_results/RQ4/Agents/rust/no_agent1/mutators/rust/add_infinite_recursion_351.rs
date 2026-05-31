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

pub struct Add_Infinite_Recursion_351;

impl Mutator for Add_Infinite_Recursion_351 {
    fn name(&self) -> &str {
        "Add_Infinite_Recursion_351"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let func_name = func.sig.ident.clone();
                let recursive_call: Expr = parse_quote! {
                    #func_name()
                };
                func.block.stmts.push(Stmt::Expr(recursive_call, None));
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator appends an infinite recursive call to each non-main function. This transformation is designed to stress-test the compiler's handling of recursion, stack management, and potential optimizations related to tail-call recursion. It may lead to compiler hangs or stack overflow errors during compilation or execution."
    }
}