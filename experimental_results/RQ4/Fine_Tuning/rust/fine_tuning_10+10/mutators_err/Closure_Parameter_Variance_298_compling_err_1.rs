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

pub struct Closure_Parameter_Variance_298;

impl Mutator for Closure_Parameter_Variance_298 {
    fn name(&self) -> &str {
        "Closure_Parameter_Variance_298"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let Some(lifetime) = item_fn.sig.generics.lifetimes().next() {
                    for input in &mut item_fn.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            if let Type::Reference(type_ref) = &*pat_type.ty {
                                let original_type = type_ref.elem.clone();
                                let new_ident = Ident::new("closure_input", Span::call_site());
                                let closure_type: Type = parse_quote! {
                                    impl FnOnce() -> #original_type
                                };
                                *pat_type.ty = Box::new(closure_type);
                                *pat_type.pat = parse_quote! { #new_ident };
                                
                                let mut new_stmts = Vec::new();
                                for stmt in &item_fn.block.stmts {
                                    if let Stmt::Expr(Expr::Path(expr_path)) = stmt {
                                        if expr_path.path.is_ident(&pat_type.pat) {
                                            let call_expr: Expr = parse_quote! {
                                                #new_ident()
                                            };
                                            new_stmts.push(Stmt::Expr(call_expr));
                                            continue;
                                        }
                                    }
                                    new_stmts.push(stmt.clone());
                                }
                                item_fn.block.stmts = new_stmts;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with lifetime parameters. It identifies a function parameter that is a reference type and replaces it with a closure capturing the lifetime. The closure returns a reference of the original type, and the function body is updated to use the closure call instead of the original parameter."
    }
}