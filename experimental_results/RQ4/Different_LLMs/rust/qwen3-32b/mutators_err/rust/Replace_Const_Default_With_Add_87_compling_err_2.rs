use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprBlock, ExprCall, ExprPath, ExprConst, ExprLit, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Replace_Const_Default_With_Add_87;

impl Mutator for Replace_Const_Default_With_Add_87 {
    fn name(&self) -> &str {
        "Replace_Const_Default_With_Add_87"
    }
    fn mutate(&self, file: &mut syn::File) {
        let add_fn_name = Ident::new("add", Span::call_site());
        let has_add_fn = file.items.iter().any(|item| {
            if let syn::Item::Fn(func) = item {
                func.sig.ident == add_fn_name && func.sig.constness.is_some()
            } else {
                false
            }
        });

        if !has_add_fn {
            let add_fn = parse_quote! {
                pub const fn add(a: usize, b: usize) -> usize {
                    a + b
                }
            };
            file.items.insert(0, syn::Item::Fn(add_fn));
        }

        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                for param in &mut struct_item.generics.params {
                    if let syn::GenericParam::Const(const_param) = param {
                        if let Some(default) = &mut const_param.default {
                            if let Expr::Block(expr_block) = &*default {
                                if expr_block.block.stmts.is_empty() {
                                    if let Some(expr) = &expr_block.block.expr {
                                        if let Expr::Const(expr_const) = &**expr {
                                            if let Expr::Lit(expr_lit) = &*expr_const.block.expr {
                                                if let Lit::Int(lit_int) = &expr_lit.lit {
                                                    let value: u64 = lit_int.base10_parse().unwrap();
                                                    let a = value / 2;
                                                    let b = value - a;
                                                    let new_expr = parse_quote! { add(#a, #b) };
                                                    let new_default = parse_quote! { const { #new_expr } };
                                                    *default = new_default;
                                                }
                                            }
                                        }
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