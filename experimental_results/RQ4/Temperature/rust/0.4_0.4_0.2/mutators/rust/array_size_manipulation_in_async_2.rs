use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, LocalInit, Pat, PatType, Path as SynPath,
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

pub struct Array_Size_Manipulation_In_Async_2;

impl Mutator for Array_Size_Manipulation_In_Async_2 {
    fn name(&self) -> &str {
        "Array_Size_Manipulation_In_Async_2"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.asyncness.is_some() {
                    for stmt in &mut item_fn.block.stmts {
                        if let Stmt::Local(local) = stmt {
                            if let Some(init) = &local.init {
                                if let Expr::Array(expr_array) = &*init.expr {
                                    if let Some(Expr::Lit(expr_lit)) = expr_array.elems.first() {
                                        if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                            let new_size: Expr = if thread_rng().gen_bool(0.5) {
                                                parse_quote!(usize::MAX)
                                            } else {
                                                parse_quote!(0)
                                            };
                                            let new_array: Expr = parse_quote!([#expr_lit; #new_size]);
                                            *stmt = Stmt::Local(syn::Local {
                                                init: Some(LocalInit {
                                                    eq_token: token::Eq::default(),
                                                    expr: Box::new(new_array),
                                                    diverge: None,
                                                }),
                                                ..local.clone()
                                            });
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
        "The mutation operator targets array size declarations within async functions. It modifies the size of arrays to either `usize::MAX` or `0`, testing the compiler's handling of extreme array sizes in async contexts. This transformation aims to uncover potential issues related to memory management or overflow in async functions dealing with arrays."
    }
}