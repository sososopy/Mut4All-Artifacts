use crate::mutator::Mutator;
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
pub struct Remove_Last_Match_Arm_3;
impl Mutator for Remove_Last_Match_Arm_3 {
    fn name(&self) -> &str {
        "Remove_Last_Match_Arm_3"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let block = &mut func.block;
                for stmt in &mut block.stmts {
                    if let syn::Stmt::Expr(expr, _) = stmt {
                        if let syn::Expr::Match(expr_match) = expr {
                            if !expr_match.arms.is_empty() {
                                expr_match.arms.pop();
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(tag1) = item {
                for impl_item in &mut tag1.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        let block = &mut method.block;
                        for stmt in &mut block.stmts {
                            if let syn::Stmt::Expr(expr, _) = stmt {
                                if let syn::Expr::Match(expr_match) = expr {
                                    if !expr_match.arms.is_empty() {
                                        expr_match.arms.pop();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets all non-main functions in the source file, including both free functions and methods within impl blocks. It replaces any explicit return type with the underscore `_` type, which requires the compiler to infer the return type. This transformation leverages Rust's type inference system to increase AST ambiguity and stress-test the compiler's ability to resolve types, particularly in more complex return scenarios involving traits, generics, and lifetimes."
    }
}
