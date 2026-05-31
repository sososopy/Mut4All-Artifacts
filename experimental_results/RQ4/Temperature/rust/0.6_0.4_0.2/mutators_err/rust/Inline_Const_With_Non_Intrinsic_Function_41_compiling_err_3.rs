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

pub struct Inline_Const_With_Non_Intrinsic_Function_41;

impl Mutator for Inline_Const_With_Non_Intrinsic_Function_41 {
    fn name(&self) -> &str {
        "Inline_Const_With_Non_Intrinsic_Function_41"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_replacement_function = false;
        let replacement_function_name = Ident::new("some_function", Span::call_site());
        let replacement_function: ItemFn = parse_quote! {
            fn some_function() -> u8 {
                42
            }
        };

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.ident == replacement_function_name {
                    has_replacement_function = true;
                }
            }
        }

        let mut new_items = Vec::new();

        for item in &mut file.items {
            if let Item::Const(item_const) = item {
                if let Expr::Block(expr_block) = &*item_const.expr {
                    let mut new_expr = None;
                    for stmt in &expr_block.block.stmts {
                        if let Stmt::Expr(Expr::Call(ExprCall { func, .. })) = stmt {
                            if let Expr::Path(ExprPath { path, .. }) = &**func {
                                if path.is_ident("core::mem::transmute") {
                                    new_expr = Some(parse_quote! {
                                        #replacement_function_name()
                                    });
                                }
                            }
                        }
                    }
                    if let Some(expr) = new_expr {
                        new_items.push((item_const.clone(), expr));
                    }
                }
            }
        }

        for (item_const, new_expr) in new_items {
            if let Item::Const(mut item_const) = item_const {
                item_const.expr = Box::new(new_expr);
                file.items.push(Item::Const(item_const));
            }
        }

        if !has_replacement_function {
            file.items.push(Item::Fn(replacement_function));
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets inline constant blocks using intrinsic functions like `core::mem::transmute`. It replaces such intrinsic calls with calls to a non-intrinsic function, `some_function`, which is either pre-existing or newly introduced. This tests the compiler's handling of inline constants when they involve regular function calls, potentially revealing issues in constant evaluation or intrinsic handling."
    }
}