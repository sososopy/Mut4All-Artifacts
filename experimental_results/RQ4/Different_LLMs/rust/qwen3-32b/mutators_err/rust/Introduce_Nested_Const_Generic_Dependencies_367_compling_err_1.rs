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

pub struct Introduce_Nested_Const_Generic_Dependencies_367;

impl Mutator for Introduce_Nested_Const_Generic_Dependencies_367 {
    fn name(&self) -> &str {
        "Introduce_Nested_Const_Generic_Dependencies_367"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, ty) = &mut func.sig.output {
                    if let syn::Type::Array(array_type) = &**ty {
                        if let syn::Expr::Lit(expr_lit) = &array_type.len {
                            if let syn::Lit::Int(_) = expr_lit.lit {
                                func.sig.generics.params.push(syn::parse_quote!(const D: usize));
                                array_type.len = syn::parse_quote!(D);
                                if let Some(block) = &mut func.block {
                                    let stmt: Stmt = syn::parse_quote!(let _ = D;);
                                    block.stmts.insert(0, stmt);
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if let syn::ReturnType::Type(_, ty) = &mut method.sig.output {
                            if let syn::Type::Array(array_type) = &**ty {
                                if let syn::Expr::Lit(expr_lit) = &array_type.len {
                                    if let syn::Lit::Int(_) = expr_lit.lit {
                                        method.sig.generics.params.push(syn::parse_quote!(const D: usize));
                                        array_type.len = syn::parse_quote!(D);
                                        if let Some(block) = &mut method.block {
                                            let stmt: Stmt = syn::parse_quote!(let _ = D;);
                                            block.stmts.insert(0, stmt);
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
        "The mutation operator introduces nested const generic dependencies by replacing fixed array lengths in function/method return types with generic const parameters. It adds a dummy reference to the const parameter in the body to force const evaluation, increasing interdependencies between const generics and associated trait constants. This stresses the compiler's const evaluation system and may trigger cycle detection or deadlock during parallel compilation."
    }
}