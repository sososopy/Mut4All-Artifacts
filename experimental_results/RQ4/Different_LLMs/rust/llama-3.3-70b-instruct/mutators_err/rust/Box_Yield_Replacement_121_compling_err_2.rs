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

pub struct Box_Yield_Replacement_121;

impl Mutator for Box_Yield_Replacement_121 {
    fn name(&self) -> &str {
        "Box_Yield_Replacement_121"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Call(call) = expr {
                            if let Expr::Path(path) = &*call.func {
                                if path.path.segments.last().unwrap().ident == "yield" {
                                    if let Some(box_token) = call.func.as_ref().map(|e| e.span().join(path.path.span()).lo()) {
                                        call.func = parse_quote! { yield };
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(func) = impl_item {
                        for stmt in &mut func.block.stmts {
                            if let Stmt::Expr(expr, _) = stmt {
                                if let Expr::Call(call) = expr {
                                    if let Expr::Path(path) = &*call.func {
                                        if path.path.segments.last().unwrap().ident == "yield" {
                                            if let Some(box_token) = call.func.as_ref().map(|e| e.span().join(path.path.span()).lo()) {
                                                call.func = parse_quote! { yield };
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
        "The Box_Yield_Replacement_121 mutation operator targets the use of the yield keyword within a closure or generator, specifically when used in conjunction with the box keyword. It replaces occurrences of 'box yield' with 'yield' to test how the compiler handles the change in the context of coroutines and generator syntax."
    }
}