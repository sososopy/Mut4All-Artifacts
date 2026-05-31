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

pub struct Replace_Return_With_Malformed_Unsafe_Closure_106;

impl Mutator for Replace_Return_With_Malformed_Unsafe_Closure_106 {
    fn name(&self) -> &str {
        "Replace_Return_With_Malformed_Unsafe_Closure_106"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let syn::ReturnType::Default = func.sig.output {
                    continue;
                }
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Return(ret) = &*expr {
                            if let Some(expr) = &ret.expr {
                                let closure = parse_quote! {
                                    unsafe { |x, y| { compute(x, y) } }
                                };
                                *expr = closure;
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if method.sig.ident == "main" {
                            continue;
                        }
                        if let syn::ReturnType::Default = method.sig.output {
                            continue;
                        }
                        for stmt in &mut method.block.stmts {
                            if let Stmt::Expr(expr, _) = stmt {
                                if let Expr::Return(ret) = &*expr {
                                    if let Some(expr) = &ret.expr {
                                        let closure = parse_quote! {
                                            unsafe { |x, y| { compute(x, y) } }
                                        };
                                        *expr = closure;
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
        ""
    }
}