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

pub struct Transform_Loop_To_Recursion_373;

impl Mutator for Transform_Loop_To_Recursion_373 {
    fn name(&self) -> &str {
        "Transform_Loop_To_Recursion_373"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let mut has_loop = false;
                for stmt in &func.block.stmts {
                    if let Stmt::Expr(Expr::Loop(_), _) = stmt {
                        has_loop = true;
                        break;
                    }
                }
                if has_loop {
                    let mut new_stmts = Vec::new();
                    let mut recursion_fn: ItemFn = parse_quote! {
                        fn recurse() {
                            // original loop body
                        }
                    };
                    recursion_fn.sig.ident = Ident::new(&format!("{}_recurse", func.sig.ident), Span::call_site());
                    recursion_fn.block.stmts = func.block.stmts.clone();
                    new_stmts.push(Stmt::Item(Item::Fn(recursion_fn)));
                    new_stmts.push(parse_quote! {
                        recurse();
                    });
                    func.block.stmts = new_stmts;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions containing loops and transforms them into recursive function calls. By converting iterative constructs into recursive ones, it tests the compiler's handling of recursion, stack management, and potential overflow scenarios. This transformation may lead to ICEs or stack overflow errors if the recursion depth exceeds the stack limit."
    }
}