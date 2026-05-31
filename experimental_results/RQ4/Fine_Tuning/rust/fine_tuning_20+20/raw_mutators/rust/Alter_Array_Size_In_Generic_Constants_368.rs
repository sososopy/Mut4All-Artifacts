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

pub struct Alter_Array_Size_In_Generic_Constants_368;

impl Mutator for Alter_Array_Size_In_Generic_Constants_368 {
    fn name(&self) -> &str {
        "Alter_Array_Size_In_Generic_Constants_368"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut has_generic_const = false;
                for param in &func.sig.generics.params {
                    if let syn::GenericParam::Const(_) = param {
                        has_generic_const = true;
                        break;
                    }
                }
                if !has_generic_const {
                    continue;
                }
                if let syn::ReturnType::Type(_, ty) = &mut func.sig.output {
                    if let syn::Type::Array(type_array) = &mut **ty {
                        if let syn::Expr::Path(expr_path) = &*type_array.len {
                            if expr_path.path.segments.len() == 1
                                && expr_path.path.segments[0].ident == "N"
                            {
                                type_array.len = Box::new(syn::Expr::Binary(syn::ExprBinary {
                                    attrs: Vec::new(),
                                    left: Box::new(syn::Expr::Path(expr_path.clone())),
                                    op: syn::BinOp::Mul(syn::token::Star::default()),
                                    right: Box::new(syn::Expr::Lit(syn::ExprLit {
                                        attrs: Vec::new(),
                                        lit: syn::Lit::Int(syn::LitInt::new(
                                            "2",
                                            proc_macro2::Span::call_site(),
                                        )),
                                    })),
                                }));
                            }
                        }
                    }
                }
                for input in &mut func.sig.inputs {
                    if let syn::FnArg::Typed(pat_type) = input {
                        if let syn::Type::Array(type_array) = &mut *pat_type.ty {
                            if let syn::Expr::Path(expr_path) = &*type_array.len {
                                if expr_path.path.segments.len() == 1
                                    && expr_path.path.segments[0].ident == "N"
                                {
                                    type_array.len = Box::new(syn::Expr::Binary(syn::ExprBinary {
                                        attrs: Vec::new(),
                                        left: Box::new(syn::Expr::Path(expr_path.clone())),
                                        op: syn::BinOp::Mul(syn::token::Star::default()),
                                        right: Box::new(syn::Expr::Lit(syn::ExprLit {
                                            attrs: Vec::new(),
                                            lit: syn::Lit::Int(syn::LitInt::new(
                                                "2",
                                                proc_macro2::Span::call_site(),
                                            )),
                                        })),
                                    }));
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                let mut has_generic_const = false;
                for param in &item_impl.generics.params {
                    if let syn::GenericParam::Const(_) = param {
                        has_generic_const = true;
                        break;
                    }
                }
                if !has_generic_const {
                    continue;
                }
                if let syn::Type::Array(type_array) = &mut *item_impl.self_ty {
                    if let syn::Expr::Path(expr_path) = &*type_array.len {
                        if expr_path.path.segments.len() == 1
                            && expr_path.path.segments[0].ident == "N"
                        {
                            type_array.len = Box::new(syn::Expr::Binary(syn::ExprBinary {
                                attrs: Vec::new(),
                                left: Box::new(syn::Expr::Path(expr_path.clone())),
                                op: syn::BinOp::Mul(syn::token::Star::default()),
                                right: Box::new(syn::Expr::Lit(syn::ExprLit {
                                    attrs: Vec::new(),
                                    lit: syn::Lit::Int(syn::LitInt::new(
                                        "2",
                                        proc_macro2::Span::call_site(),
                                    )),
                                })),
                            }));
                        }
                    }
                }
                for item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(method) = item {
                        if let syn::ReturnType::Type(_, ty) = &mut method.sig.output {
                            if let syn::Type::Array(type_array) = &mut **ty {
                                if let syn::Expr::Path(expr_path) = &*type_array.len {
                                    if expr_path.path.segments.len() == 1
                                        && expr_path.path.segments[0].ident == "N"
                                    {
                                        type_array.len = Box::new(syn::Expr::Binary(
                                            syn::ExprBinary {
                                                attrs: Vec::new(),
                                                left: Box::new(syn::Expr::Path(
                                                    expr_path.clone(),
                                                )),
                                                op: syn::BinOp::Mul(syn::token::Star::default()),
                                                right: Box::new(syn::Expr::Lit(syn::ExprLit {
                                                    attrs: Vec::new(),
                                                    lit: syn::Lit::Int(syn::LitInt::new(
                                                        "2",
                                                        proc_macro2::Span::call_site(),
                                                    )),
                                                })),
                                            },
                                        ));
                                    }
                                }
                            }
                        }
                        for input in &mut method.sig.inputs {
                            if let syn::FnArg::Typed(pat_type) = input {
                                if let syn::Type::Array(type_array) = &mut *pat_type.ty {
                                    if let syn::Expr::Path(expr_path) = &*type_array.len {
                                        if expr_path.path.segments.len() == 1
                                            && expr_path.path.segments[0].ident == "N"
                                        {
                                            type_array.len = Box::new(syn::Expr::Binary(
                                                syn::ExprBinary {
                                                    attrs: Vec::new(),
                                                    left: Box::new(syn::Expr::Path(
                                                        expr_path.clone(),
                                                    )),
                                                    op: syn::BinOp::Mul(
                                                        syn::token::Star::default(),
                                                    ),
                                                    right: Box::new(syn::Expr::Lit(syn::ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Int(syn::LitInt::new(
                                                            "2",
                                                            proc_macro2::Span::call_site(),
                                                        )),
                                                    })),
                                                },
                                            ));
                                        }
                                    }
                                }
                            }
                        }
                        for stmt in &mut method.block.stmts {
                            if let syn::Stmt::Expr(expr, _) = stmt {
                                if let syn::Expr::Return(expr_return) = expr {
                                    if let Some(expr) = &expr_return.expr {
                                        if let syn::Expr::Array(expr_array) = &**expr {
                                            let mut new_elems = Punctuated::new();
                                            for elem in &expr_array.elems {
                                                new_elems.push(elem.clone());
                                            }
                                            for elem in &expr_array.elems {
                                                new_elems.push(elem.clone());
                                            }
                                            *stmt = syn::Stmt::Expr(
                                                syn::Expr::Return(syn::ExprReturn {
                                                    attrs: Vec::new(),
                                                    return_token: syn::token::Return::default(),
                                                    expr: Some(Box::new(syn::Expr::Array(
                                                        syn::ExprArray {
                                                            attrs: Vec::new(),
                                                            bracket_token:
                                                                syn::token::Bracket::default(),
                                                            elems: new_elems,
                                                        },
                                                    ))),
                                                }),
                                                None,
                                            );
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
        "This mutator targets functions and impls that use arrays with generic constants, specifically looking for array types that depend on a generic constant `N`. It doubles the size of such arrays in both function signatures and impl headers, and also modifies the return expressions to match the new size by duplicating array elements. This transformation stresses the compiler's handling of generic constants, array sizing, and return type consistency, potentially exposing weaknesses in type inference and const evaluation."
    }
}