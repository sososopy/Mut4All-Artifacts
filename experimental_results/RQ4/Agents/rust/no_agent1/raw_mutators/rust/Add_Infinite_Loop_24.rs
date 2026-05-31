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

pub struct Add_Infinite_Loop_24;

impl Mutator for Add_Infinite_Loop_24 {
    fn name(&self) -> &str {
        "Add_Infinite_Loop_24"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    let infinite_loop: Stmt = parse_quote! {
                        loop {
                            // Infinite loop intended to stress test the compiler
                        }
                    };
                    func.block.stmts.insert(0, infinite_loop);
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.ident != "main" {
                            let infinite_loop: Stmt = parse_quote! {
                                loop {
                                    // Infinite loop intended to stress test the compiler
                                }
                            };
                            func.block.stmts.insert(0, infinite_loop);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator inserts an infinite loop at the beginning of non-main function and method bodies. The intent is to stress test the compiler's handling of control flow and loop optimization. It may trigger compiler hangs, stack overflow errors, or ICEs due to the infinite nature of the loop and its placement in the code."
    }
}