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

pub struct Alter_Usage_Of_Macros_With_Const_Generics_110;

impl Mutator for Alter_Usage_Of_Macros_With_Const_Generics_110 {
    fn name(&self) -> &str {
        "Alter_Usage_Of_Macros_With_Const_Generics_110"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_target = false;
        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                let (_, generics, _) = item_struct.generics.split_for_impl();
                for generic in generics {
                    if let GenericParam::Const(const_param) = generic {
                        if let Some(expr) = &const_param.default {
                            if let Expr::Block(expr_block) = expr {
                                if expr_block.block.stmts.len() == 1 {
                                    if let Stmt::Expr(Expr::Macro(_), _) =
                                        &expr_block.block.stmts[0]
                                    {
                                        has_target = true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if !has_target {
            return;
        }
        let mut outer_wrap = false;
        for item in &file.items {
            if let Item::Macro(item_macro) = item {
                if item_macro.ident == Some(Ident::new("outer_wrap", Span::call_site())) {
                    outer_wrap = true;
                }
            }
        }
        if !outer_wrap {
            let new_macro: ItemMacro = parse_quote! {
                macro_rules! outer_wrap {
                    ($x:expr) => {
                        macro!($x)
                    };
                }
            };
            file.items.push(Item::Macro(new_macro));
        }
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let (_, generics, _) = item_struct.generics.split_for_impl();
                for generic in generics {
                    if let GenericParam::Const(const_param) = generic {
                        if let Some(expr) = &const_param.default {
                            if let Expr::Block(expr_block) = expr {
                                if expr_block.block.stmts.len() == 1 {
                                    if let Stmt::Expr(Expr::Macro(expr_macro), _) =
                                        &expr_block.block.stmts[0]
                                    {
                                        let new_expr: Expr = parse_quote! {
                                            {
                                                outer_wrap! (#expr_macro)
                                            }
                                        };
                                        *expr = new_expr;
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
        "By introducing an additional layer of macro wrapping around a const generic parameter, the mutation operator seeks to stress the Rust compiler's handling of macro expansions in the context of const generics, which is a known fragile area, as indicated by the bug report. This can potentially lead to internal compiler errors or unexpected behavior. The mutation specifically targets the interplay between macros and const generics, exploiting the complexity that arises from nested macro calls within const contexts."
    }
}