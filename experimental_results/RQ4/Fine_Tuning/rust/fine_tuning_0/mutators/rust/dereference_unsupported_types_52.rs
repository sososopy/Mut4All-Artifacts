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

pub struct Dereference_Unsupported_Types_52;

impl Mutator for Dereference_Unsupported_Types_52 {
    fn name(&self) -> &str {
        "Dereference_Unsupported_Types_52"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Const(item_const) = item {
                if let Type::Tuple(type_tuple) = &*item_const.ty {
                    if type_tuple.elems.is_empty() {
                        let expr = Expr::Tuple(syn::ExprTuple {
                            attrs: Vec::new(),
                            paren_token: token::Paren::default(),
                            elems: Punctuated::new(),
                        });
                        let deref_expr = Expr::Unary(syn::ExprUnary {
                            attrs: Vec::new(),
                            op: syn::UnOp::Deref(token::Star::default()),
                            expr: Box::new(expr),
                        });
                        item_const.expr = Box::new(deref_expr);
                    }
                }
                if let Type::Path(type_path) = &*item_const.ty {
                    if type_path.path.is_ident("str") {
                        let expr = Expr::Lit(syn::ExprLit {
                            attrs: Vec::new(),
                            lit: syn::Lit::Str(syn::LitStr::new("", Span::call_site())),
                        });
                        let deref_expr = Expr::Unary(syn::ExprUnary {
                            attrs: Vec::new(),
                            op: syn::UnOp::Deref(token::Star::default()),
                            expr: Box::new(expr),
                        });
                        item_const.expr = Box::new(deref_expr);
                    }
                }
            }
            if let Item::Static(item_static) = item {
                if let Type::Tuple(type_tuple) = &*item_static.ty {
                    if type_tuple.elems.is_empty() {
                        let expr = Expr::Tuple(syn::ExprTuple {
                            attrs: Vec::new(),
                            paren_token: token::Paren::default(),
                            elems: Punctuated::new(),
                        });
                        let deref_expr = Expr::Unary(syn::ExprUnary {
                            attrs: Vec::new(),
                            op: syn::UnOp::Deref(token::Star::default()),
                            expr: Box::new(expr),
                        });
                        item_static.expr = Box::new(deref_expr);
                    }
                }
                if let Type::Path(type_path) = &*item_static.ty {
                    if type_path.path.is_ident("str") {
                        let expr = Expr::Lit(syn::ExprLit {
                            attrs: Vec::new(),
                            lit: syn::Lit::Str(syn::LitStr::new("", Span::call_site())),
                        });
                        let deref_expr = Expr::Unary(syn::ExprUnary {
                            attrs: Vec::new(),
                            op: syn::UnOp::Deref(token::Star::default()),
                            expr: Box::new(expr),
                        });
                        item_static.expr = Box::new(deref_expr);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets constant and static variables of unsupported types like `()` and `str`, applying a dereference operation to provoke compilation errors. It explores the compiler's handling of invalid dereference operations, potentially triggering ICEs or other compiler errors."
    }
}