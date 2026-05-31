use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet,
    fs,
    ops::Range,
    panic,
    path::Path,
    process::Command,
    mem,
};
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

pub struct Wrap_Const_Generic_Default_In_Block_350;

impl Mutator for Wrap_Const_Generic_Default_In_Block_350 {
    fn name(&self) -> &str {
        "Wrap_Const_Generic_Default_In_Block_350"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(s) = item {
                for param in &mut s.generics.params {
                    if let syn::GenericParam::Const(const_param) = param {
                        if let Some(default_expr) = &mut const_param.default {
                            let original_expr = mem::replace(default_expr, Box::new(parse_quote! { 0 }));
                            if let syn::Expr::Const(expr_const) = *original_expr {
                                let block = syn::Block {
                                    brace_token: syn::token::Brace::default(),
                                    stmts: {
                                        let mut stmts = Vec::new();
                                        stmts.push(syn::Stmt::Expr(expr_const, None));
                                        stmts
                                    },
                                };
                                let expr_block = syn::Expr::Block(syn::ExprBlock {
                                    attrs: Vec::new(),
                                    label: None,
                                    block,
                                });
                                *default_expr = Box::new(expr_block);
                            } else {
                                *default_expr = original_expr;
                            }
                        }
                    }
                }
            } else if let syn::Item::Enum(e) = item {
                for param in &mut e.generics.params {
                    if let syn::GenericParam::Const(const_param) = param {
                        if let Some(default_expr) = &mut const_param.default {
                            let original_expr = mem::replace(default_expr, Box::new(parse_quote! { 0 }));
                            if let syn::Expr::Const(expr_const) = *original_expr {
                                let block = syn::Block {
                                    brace_token: syn::token::Brace::default(),
                                    stmts: {
                                        let mut stmts = Vec::new();
                                        stmts.push(syn::Stmt::Expr(expr_const, None));
                                        stmts
                                    },
                                };
                                let expr_block = syn::Expr::Block(syn::ExprBlock {
                                    attrs: Vec::new(),
                                    label: None,
                                    block,
                                });
                                *default_expr = Box::new(expr_block);
                            } else {
                                *default_expr = original_expr;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const generic parameters in structs and enums that have default values expressed as const blocks. It wraps these const blocks in an additional redundant block, increasing the nesting level of the expression. This transformation may expose compiler bugs in handling nested const contexts during substitution and evaluation of generic parameters."
    }
}