use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    parse_quote, spanned::Spanned, visit_mut::VisitMut, Attribute, Expr, ExprCall, ExprPath, File,
    FnArg, GenericArgument, GenericParam, Ident, Item, ItemFn, ItemStruct, Lifetime, LifetimeParam,
    Local, LocalInit, Pat, PatType, Path as SynPath, PathArguments, ReturnType, Stmt, TraitBound,
    TraitBoundModifier, Type, TypeImplTrait, TypeParamBound, TypePath, punctuated::Punctuated,
    token, token::Comma, token::Paren, token::Plus, ExprAssign,
};

use crate::mutator::Mutator;

pub struct Attribute_On_Expressions_25;

impl Mutator for Attribute_On_Expressions_25 {
    fn name(&self) -> &str {
        "Attribute_On_Expressions_25"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(init) = &local.init {
                            let attr: Attribute = parse_quote!(#[unstable_feature]);
                            let new_expr = Expr::Assign(ExprAssign {
                                attrs: vec![attr],
                                left: Box::new(init.expr.clone()),
                                eq_token: token::Eq { spans: [Span::call_site()] },
                                right: Box::new(init.expr.clone()),
                            });
                            local.init = Some(LocalInit {
                                eq_token: init.eq_token,
                                expr: Box::new(new_expr),
                                diverge: init.diverge.clone(),
                            });
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces an unstable attribute directly onto expressions within local variable initializations. This transformation attempts to expose potential parsing issues or internal compiler errors by using attributes in non-standard locations, challenging the compiler's attribute handling mechanisms."
    }
}