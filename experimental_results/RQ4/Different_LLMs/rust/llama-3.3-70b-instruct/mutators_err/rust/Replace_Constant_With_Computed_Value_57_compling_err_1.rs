use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Replace_Constant_With_Computed_Value_57;

impl Mutator for Replace_Constant_With_Computed_Value_57 {
    fn name(&self) -> &str {
        "Replace_Constant_With_Computed_Value_57"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Const(const_item) = item {
                let computed_value = self.generate_computed_value(const_item.ty.clone());
                const_item.expr = computed_value;
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces constant values with computed values. This transformation aims to test the compiler's handling of constant evaluation and may lead to bugs related to constant evaluation."
    }
}

impl Replace_Constant_With_Computed_Value_57 {
    fn generate_computed_value(&self, ty: syn::Type) -> syn::Expr {
        match ty {
            syn::Type::Path(syn::TypePath { qself: None, path }) => {
                let ident = path.segments.last().unwrap().ident.clone();
                match ident.to_string().as_str() {
                    "i32" => self.generate_i32_computed_value(),
                    "bool" => self.generate_bool_computed_value(),
                    "char" => self.generate_char_computed_value(),
                    _ => self.generate_default_computed_value(),
                }
            }
            _ => self.generate_default_computed_value(),
        }
    }

    fn generate_i32_computed_value(&self) -> syn::Expr {
        let func_name = Ident::new("generate_i32", Span::call_site());
        let func_call = ExprCall {
            attrs: vec![],
            expr: Box::new(ExprPath {
                attrs: vec![],
                qself: None,
                path: SynPath {
                    leading_colon: None,
                    segments: Punctuated::from_iter(vec![PathSegment {
                        ident: func_name,
                        arguments: PathArguments::None,
                    }]),
                },
            }),
            paren_token: Paren {
                span: Span::call_site(),
            },
            args: punctuated::Punctuated::new(),
        };
        func_call
    }

    fn generate_bool_computed_value(&self) -> syn::Expr {
        let func_name = Ident::new("generate_bool", Span::call_site());
        let func_call = ExprCall {
            attrs: vec![],
            expr: Box::new(ExprPath {
                attrs: vec![],
                qself: None,
                path: SynPath {
                    leading_colon: None,
                    segments: Punctuated::from_iter(vec![PathSegment {
                        ident: func_name,
                        arguments: PathArguments::None,
                    }]),
                },
            }),
            paren_token: Paren {
                span: Span::call_site(),
            },
            args: punctuated::Punctuated::new(),
        };
        func_call
    }

    fn generate_char_computed_value(&self) -> syn::Expr {
        let func_name = Ident::new("generate_char", Span::call_site());
        let func_call = ExprCall {
            attrs: vec![],
            expr: Box::new(ExprPath {
                attrs: vec![],
                qself: None,
                path: SynPath {
                    leading_colon: None,
                    segments: Punctuated::from_iter(vec![PathSegment {
                        ident: func_name,
                        arguments: PathArguments::None,
                    }]),
                },
            }),
            paren_token: Paren {
                span: Span::call_site(),
            },
            args: punctuated::Punctuated::new(),
        };
        func_call
    }

    fn generate_default_computed_value(&self) -> syn::Expr {
        let func_name = Ident::new("generate_default", Span::call_site());
        let func_call = ExprCall {
            attrs: vec![],
            expr: Box::new(ExprPath {
                attrs: vec![],
                qself: None,
                path: SynPath {
                    leading_colon: None,
                    segments: Punctuated::from_iter(vec![PathSegment {
                        ident: func_name,
                        arguments: PathArguments::None,
                    }]),
                },
            }),
            paren_token: Paren {
                span: Span::call_site(),
            },
            args: punctuated::Punctuated::new(),
        };
        func_call
    }
}