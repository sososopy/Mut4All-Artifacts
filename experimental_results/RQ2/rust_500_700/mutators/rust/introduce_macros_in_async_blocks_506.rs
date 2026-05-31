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

pub struct Introduce_Macros_In_Async_Blocks_506;

impl Mutator for Introduce_Macros_In_Async_Blocks_506 {
    fn name(&self) -> &str {
        "Introduce_Macros_In_Async_Blocks_506"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut macro_defined = false;

        for item in &mut file.items {
            if let Item::Macro(macro_item) = item {
                if let Some(ident) = &macro_item.ident {
                    if ident == "log_and_await" {
                        macro_defined = true;
                        break;
                    }
                }
            }
        }

        if !macro_defined {
            let macro_def: ItemMacro = parse_quote! {
                macro_rules! log_and_await {
                    ($expr:expr) => {
                        {
                            println!("Awaiting: {:?}", stringify!($expr));
                            $expr.await
                        }
                    };
                }
            };
            file.items.insert(0, Item::Macro(macro_def));
        }

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = AsyncBlockVisitor;
                visitor.visit_block_mut(&mut item_fn.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies async blocks and introduces a macro that wraps around `await` expressions. This transformation aims to test the compiler's handling of macros within async contexts, focusing on the interaction between macro expansion and async state machines."
    }
}

struct AsyncBlockVisitor;

impl VisitMut for AsyncBlockVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Await(expr_await) = expr {
            let new_expr: Expr = parse_quote! {
                log_and_await!(#expr_await.base)
            };
            *expr = new_expr;
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}