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

pub struct Replace_Const_Fn_With_Invalid_360;

impl Mutator for Replace_Const_Fn_With_Invalid_360 {
    fn name(&self) -> &str {
        "Replace_Const_Fn_With_Invalid_360"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Const(const_item) = impl_item {
                        if let syn::Expr::Call(expr_call) = &const_item.expr {
                            if let syn::Expr::Path(expr_path) = &expr_call.func {
                                if let Some(segment) = expr_path.path.segments.last() {
                                    if segment.ident == "compute" {
                                        const_item.expr = parse_quote! { 100 };
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
        "The mutation operator targets const function calls within trait implementations using const generics. It replaces these calls with a direct constant value that is out of the expected range or context. This transformation aims to test the compiler's handling of const generics and const function calls by introducing unexpected constant values, potentially leading to compilation errors or logical inconsistencies."
    }
}