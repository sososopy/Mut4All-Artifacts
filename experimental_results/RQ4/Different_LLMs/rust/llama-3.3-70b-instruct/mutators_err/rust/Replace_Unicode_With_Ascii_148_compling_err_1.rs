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

pub struct Replace_Unicode_With_Ascii_148;

impl Mutator for Replace_Unicode_With_Ascii_148 {
    fn name(&self) -> &str {
        "Replace_Unicode_With_Ascii_148"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr) = stmt {
                        if let Expr::Call(call) = &**expr {
                            if let Expr::Path(path) = &*call.expr {
                                let mut new_path = path.clone();
                                new_path.path.segments.iter_mut().for_each(|segment| {
                                    if let Some(ident) = segment.ident.to_string().chars().next() {
                                        if ident.is_ascii() == false {
                                            segment.ident = Ident::new(&ident.to_ascii_lowercase(), segment.ident.span());
                                        }
                                    }
                                });
                                call.expr = Box::new(Expr::Path(new_path));
                            }
                        }
                    }
                }
            }
            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(func) = impl_item {
                        for stmt in &mut func.block.stmts {
                            if let Stmt::Expr(expr) = stmt {
                                if let Expr::Call(call) = &**expr {
                                    if let Expr::Path(path) = &*call.expr {
                                        let mut new_path = path.clone();
                                        new_path.path.segments.iter_mut().for_each(|segment| {
                                            if let Some(ident) = segment.ident.to_string().chars().next() {
                                                if ident.is_ascii() == false {
                                                    segment.ident = Ident::new(&ident.to_ascii_lowercase(), segment.ident.span());
                                                }
                                            }
                                        });
                                        call.expr = Box::new(Expr::Path(new_path));
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
        "The mutation operator replaces Unicode characters with their ASCII counterparts in function and method calls. This transformation tests the compiler's handling of Unicode characters and its ability to distinguish between similar-looking characters."
    }
}