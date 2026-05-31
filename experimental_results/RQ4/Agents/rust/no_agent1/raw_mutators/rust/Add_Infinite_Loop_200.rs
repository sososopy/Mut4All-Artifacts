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

pub struct Add_Infinite_Loop_200;

impl Mutator for Add_Infinite_Loop_200 {
    fn name(&self) -> &str {
        "Add_Infinite_Loop_200"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    let infinite_loop: Stmt = parse_quote! {
                        loop {
                            // Infinite loop to stress test the compiler's handling of control flow
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
                                    // Infinite loop to stress test the compiler's handling of control flow
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
        "The mutation operator introduces an infinite loop at the beginning of non-main functions and methods. This aims to test the compiler's ability to handle potentially non-terminating control flow structures, which may lead to hangs or stack overflow errors during compilation or execution. It stresses the compiler's optimization and code generation phases by introducing unreachable code scenarios."
    }
}