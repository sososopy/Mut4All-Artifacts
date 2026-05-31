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

pub struct Nested_Coroutine_Borrow_Ice_117;

impl Mutator for Nested_Coroutine_Borrow_Ice_117 {
    fn name(&self) -> &str {
        "Nested_Coroutine_Borrow_Ice_117"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut has_mut_ref = false;
                let mut ref_ident = None;
                for input in &func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Reference(ty_ref) = &*pat_type.ty {
                            if ty_ref.mutability.is_some() {
                                if let Pat::Ident(pat_ident) = &*pat_type.pat {
                                    ref_ident = Some(pat_ident.ident.clone());
                                    has_mut_ref = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                if has_mut_ref {
                    if let Some(ident) = ref_ident {
                        let h_closure: syn::Expr = parse_quote! {
                            || {
                                let a = &mut *#ident;
                                yield;
                            }
                        };
                        let g_closure: syn::Expr = parse_quote! {
                            || {
                                let mut h = #h_closure;
                                yield;
                            }
                        };
                        let new_stmt = parse_quote! {
                            let mut g = #g_closure;
                        };
                        func.block.stmts.insert(0, new_stmt);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}