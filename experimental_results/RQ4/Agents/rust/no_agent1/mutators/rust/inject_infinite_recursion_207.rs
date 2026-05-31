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

pub struct Inject_Infinite_Recursion_207;

impl Mutator for Inject_Infinite_Recursion_207 {
    fn name(&self) -> &str {
        "Inject_Infinite_Recursion_207"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let func_name = &func.sig.ident;
                let infinite_recursion: Stmt = parse_quote! {
                    #func_name();
                };
                func.block.stmts.insert(0, infinite_recursion.clone());
                func.block.stmts.push(infinite_recursion);
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        let method_name = &method.sig.ident;
                        let infinite_recursion: Stmt = parse_quote! {
                            #method_name();
                        };
                        method.block.stmts.insert(0, infinite_recursion.clone());
                        method.block.stmts.push(infinite_recursion);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces infinite recursion by adding calls to the function itself at both the start and end of its body. This transformation is designed to test the compiler's handling of stack overflows and recursion limits, potentially triggering hangs or ICEs due to excessive recursion depth."
    }
}