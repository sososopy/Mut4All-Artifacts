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

pub struct Change_Array_Size_88;

impl Mutator for Change_Array_Size_88 {
    fn name(&self) -> &str {
        "Change_Array_Size_88"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Array(expr_array) = **expr {
                            for elem in &mut expr_array.elems {
                                if let Expr::Lit(ExprLit {
                                    lit: Lit::Int(int_lit),
                                    ..
                                }) = **elem
                                {
                                    let mut rng = thread_rng();
                                    let new_value: i32 = rng.gen_range(0..100);
                                    *int_lit = LitInt {
                                        base: 10,
                                        suffix: None,
                                        span: Span::call_site(),
                                        value: new_value,
                                    };
                                }
                            }
                        }
                    }
                }
            }

            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for stmt in &mut func.block.stmts {
                            if let Stmt::Expr(expr, _) = stmt {
                                if let Expr::Array(expr_array) = **expr {
                                    for elem in &mut expr_array.elems {
                                        if let Expr::Lit(ExprLit {
                                            lit: Lit::Int(int_lit),
                                            ..
                                        }) = **elem
                                        {
                                            let mut rng = thread_rng();
                                            let new_value: i32 = rng.gen_range(0..100);
                                            *int_lit = LitInt {
                                                base: 10,
                                                suffix: None,
                                                span: Span::call_site(),
                                                value: new_value,
                                            };
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
        "The mutation operator targets array declarations and changes the size of an array type. It applies to any code that uses constant generic expressions and array operations. The new size should be a valid constant expression that is different from the original size. This could involve changing a numeric literal, adding or removing elements from an array initializer, or modifying a constant expression that determines the array size."
    }
}