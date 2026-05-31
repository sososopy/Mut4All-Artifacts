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

pub struct Self_Reference_Fix_In_Trait_Impl_633;

impl Mutator for Self_Reference_Fix_In_Trait_Impl_633 {
    fn name(&self) -> &str {
        "Self_Reference_Fix_In_Trait_Impl_633"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        for input in &method.sig.inputs {
                            if let FnArg::Receiver(_) = input {
                                let mut new_stmts = Vec::new();
                                for stmt in &method.block.stmts {
                                    if let Stmt::Expr(Expr::Reference(expr_ref), _) = stmt {
                                        if let Expr::Path(ExprPath { ref path, .. }) = *expr_ref.expr {
                                            if path.is_ident("self") {
                                                let new_stmt: Stmt = parse_quote! {
                                                    println!("{:?}", self);
                                                };
                                                new_stmts.push(new_stmt);
                                                continue;
                                            }
                                        }
                                    }
                                    new_stmts.push(stmt.clone());
                                }
                                method.block.stmts = new_stmts;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}