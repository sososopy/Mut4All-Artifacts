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

pub struct Add_Proc_Macro_Attribute_254;

impl Mutator for Add_Proc_Macro_Attribute_254 {
    fn name(&self) -> &str {
        "Add_Proc_Macro_Attribute_254"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut target_stmt: Option<&mut Stmt> = None;
        let mut target_expr: Option<&mut Expr> = None;
        let mut target_item: Option<&mut Item> = None;

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for stmt in &mut item_fn.block.stmts {
                    if !stmt.attrs.is_empty() {
                        continue;
                    }
                    target_stmt = Some(stmt);
                    break;
                }
                if target_stmt.is_some() {
                    break;
                }
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(method) = impl_item {
                        for stmt in &mut method.block.stmts {
                            if !stmt.attrs.is_empty() {
                                continue;
                            }
                            target_stmt = Some(stmt);
                            break;
                        }
                        if target_stmt.is_some() {
                            break;
                        }
                    }
                }
                if target_stmt.is_some() {
                    break;
                }
            }
            if !item.attrs.is_empty() {
                continue;
            }
            target_item = Some(item);
            break;
        }

        if target_stmt.is_none() && target_item.is_none() {
            for item in &mut file.items {
                if let Item::Fn(item_fn) = item {
                    for stmt in &mut item_fn.block.stmts {
                        if let Stmt::Expr(expr, _) = stmt {
                            if !expr.attrs.is_empty() {
                                continue;
                            }
                            target_expr = Some(expr);
                            break;
                        }
                    }
                    if target_expr.is_some() {
                        break;
                    }
                }
                if let Item::Impl(item_impl) = item {
                    for impl_item in &mut item_impl.items {
                        if let ImplItem::Fn(method) = impl_item {
                            for stmt in &mut method.block.stmts {
                                if let Stmt::Expr(expr, _) = stmt {
                                    if !expr.attrs.is_empty() {
                                        continue;
                                    }
                                    target_expr = Some(expr);
                                    break;
                                }
                            }
                            if target_expr.is_some() {
                                break;
                            }
                        }
                    }
                    if target_expr.is_some() {
                        break;
                    }
                }
            }
        }

        if let Some(stmt) = target_stmt {
            let attr = parse_quote!(#[breaks_rust]);
            stmt.attrs.push(attr);
        } else if let Some(item) = target_item {
            let attr = parse_quote!(#[breaks_rust]);
            item.attrs.push(attr);
        } else if let Some(expr) = target_expr {
            let attr = parse_quote!(#[breaks_rust]);
            expr.attrs.push(attr);
        } else {
            return;
        }

        let mut has_proc_macro = false;
        for item in &file.items {
            if let Item::Macro(item_macro) = item {
                if item_macro.mac.path.is_ident("proc_macro") {
                    has_proc_macro = true;
                    break;
                }
            }
        }

        if !has_proc_macro {
            let proc_macro_def: Item = parse_quote! {
                use proc_macro::TokenStream;
                #[proc_macro_attribute]
                pub fn breaks_rust(_: TokenStream, input: TokenStream) -> TokenStream { input }
            };
            file.items.insert(0,proc_macro_def);
        }

        let mut has_feature = false;
        for attr in &file.attrs {
            if let Meta::List(list) = &attr.meta {
                if list.path.is_ident("feature") {
                    has_feature = true;
                    break;
                }
            }
        }

        if !has_feature {
            let feature_attr: Attribute = parse_quote!(#![feature(stmt_expr_attributes, proc_macro_hygiene)]);
            file.attrs.insert(0,feature_attr);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds a dummy proc macro attribute #[breaks_rust] to an existing statement, expression, or item in the seed program. It ensures the required features stmt_expr_attributes and proc_macro_hygiene are enabled and defines the proc macro if missing. This transformation aims to trigger internal compiler errors related to token handling for nonterminals when attributes are applied to statements or expressions."
    }
}