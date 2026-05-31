use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
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
};

use crate::mutator::Mutator;

pub struct Closure_Parameter_Variance_145;

impl Mutator for Closure_Parameter_Variance_145 {
    fn name(&self) -> &str {
        "Closure_Parameter_Variance_145"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(lifetime) = func.sig.generics.lifetimes().next() {
                    if let Some(FnArg::Typed(pat_type)) = func.sig.inputs.iter_mut().find(|arg| {
                        if let FnArg::Typed(pat_type) = arg {
                            if let Type::Reference(type_ref) = &*pat_type.ty {
                                return type_ref.lifetime.is_some();
                            }
                        }
                        false
                    }) {
                        let original_type = pat_type.ty.clone();
                        let closure_type: Type = parse_quote! {
                            impl FnOnce() -> #original_type
                        };
                        pat_type.ty = Box::new(closure_type);

                        let mut new_block = *func.block.clone();
                        for stmt in new_block.stmts.iter_mut() {
                            if let Stmt::Expr(expr, _) = stmt {
                                if let Expr::Path(expr_path) = expr {
                                    if expr_path.path.segments.len() == 1 {
                                        if let Pat::Ident(pat_ident) = &*pat_type.pat {
                                            if expr_path.path.segments[0].ident == pat_ident.ident {
                                                *expr = parse_quote! { #pat_ident() };
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        func.block = Box::new(new_block);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies functions with lifetime parameters and modifies a parameter to be a closure capturing the lifetime. It replaces the parameter with a closure type and updates the function body to call the closure, testing the compiler's handling of lifetime variance in closures."
    }
}