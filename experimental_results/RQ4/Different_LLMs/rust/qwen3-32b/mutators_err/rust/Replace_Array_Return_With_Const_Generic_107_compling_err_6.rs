use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprArray, ExprLit, ExprRepeat, ExprTuple, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeArray,
    TypeImplTrait, TypeParamBound, TypePath, parse_quote,
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

pub struct Replace_Array_Return_With_Const_Generic_107;

impl Mutator for Replace_Array_Return_With_Const_Generic_107 {
    fn name(&self) -> &str {
        "Replace_Array_Return_With_Const_Generic_107"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let syn::ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let syn::Type::Array(array_type) = return_type.as_mut() {
                        if let syn::Expr::Lit(lit) = &array_type.len {
                            if let syn::Lit::Int(lit_int) = &lit.lit {
                                let size = match lit_int.base10_parse::<usize>() {
                                    Ok(s) => s,
                                    Err(_) => continue,
                                };
                                if let Some(stmt) = func.block.stmts.last_mut() {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        if let &Expr::Array(ref array_expr) = &**expr {
                                            if array_expr.elems.len() == size {
                                                let const_param = parse_quote! { const N: usize };
                                                func.sig.generics.params.push(const_param);
                                                array_type.len = parse_quote! { N };
                                                if let Some(first_element) = array_expr.elems.get(0).cloned() {
                                                    let new_expr = parse_quote! { [ #first_element; N ] };
                                                    *expr = Box::new(new_expr);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            } else if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let syn::ReturnType::Type(_, return_type) = &mut func.sig.output {
                            if let syn::Type::Array(array_type) = return_type.as_mut() {
                                if let syn::Expr::Lit(lit) = &array_type.len {
                                    if let syn::Lit::Int(lit_int) = &lit.lit {
                                        let size = match lit_int.base10_parse::<usize>() {
                                            Ok(s) => s,
                                            Err(_) => continue,
                                        };
                                        if let Some(stmt) = func.block.stmts.last_mut() {
                                            if let Stmt::Expr(expr, _) = stmt {
                                                if let &Expr::Array(ref array_expr) = &**expr {
                                                    if array_expr.elems.len() == size {
                                                        let const_param = parse_quote! { const N: usize };
                                                        func.sig.generics.params.push(const_param);
                                                        array_type.len = parse_quote! { N };
                                                        if let Some(first_element) = array_expr.elems.get(0).cloned() {
                                                            let new_expr = parse_quote! { [ #first_element; N ] };
                                                            *expr = Box::new(new_expr);
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
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions returning fixed-size arrays with literal sizes. It introduces a const generic parameter N, modifies the return type to [T; N], and replaces the array literal in the body with a repeat expression using the first element and N. This transformation stresses the compiler's ability to resolve const generics in return types and array construction, potentially exposing bugs in type inference and const parameter handling."
    }
}