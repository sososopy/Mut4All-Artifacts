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

pub struct Modify_Enum_Variant_Pattern_241;

impl Mutator for Modify_Enum_Variant_Pattern_241 {
    fn name(&self) -> &str {
        "Modify_Enum_Variant_Pattern_241"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Expr(Expr::If(expr_if), _) = stmt {
                        if let Expr::Let(expr_let) = &*expr_if.cond {
                            if let Pat::TupleStruct(pat_tuple_struct) = &*expr_let.pat {
                                if let Expr::Lit(expr_lit) = &*expr_let.expr {
                                    if let syn::Lit::Str(_) = expr_lit.lit {
                                        let new_expr: Expr = parse_quote! { 42 };
                                        expr_if.cond = Box::new(Expr::Let(syn::ExprLet {
                                            attrs: expr_let.attrs.clone(),
                                            let_token: expr_let.let_token,
                                            pat: expr_let.pat.clone(),
                                            eq_token: expr_let.eq_token,
                                            expr: Box::new(new_expr),
                                        }));
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