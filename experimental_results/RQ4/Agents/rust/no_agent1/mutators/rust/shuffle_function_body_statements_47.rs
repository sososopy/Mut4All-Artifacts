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

pub struct Shuffle_Function_Body_Statements_47;

impl Mutator for Shuffle_Function_Body_Statements_47 {
    fn name(&self) -> &str {
        "Shuffle_Function_Body_Statements_47"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let mut rng = thread_rng();
                let statements = &mut func.block.stmts;
                statements.shuffle(&mut rng);
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let mut rng = thread_rng();
                        let statements = &mut func.block.stmts;
                        statements.shuffle(&mut rng);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator randomly shuffles the statements within function and method bodies, excluding the main function. This transformation disrupts the original logical flow of the code, potentially leading to unexpected behaviors, logic errors, or compiler crashes due to the altered execution order. It aims to test the compiler's robustness in handling code with non-deterministic statement ordering."
    }
}