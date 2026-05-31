use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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

pub struct Replace_Const_With_Arithmetic_347;

impl Mutator for Replace_Const_With_Arithmetic_347 {
    fn name(&self) -> &str {
        "Replace_Const_With_Arithmetic_347"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = MethodCallVisitor;
        visitor.visit_file_mut(file);
        
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some(const_params) = get_const_params(&item_impl.generics) {
                    for impl_item in &mut item_impl.items {
                        if let syn::ImplItem::Fn(method) = impl_item {
                            if method.sig.ident != "main" {
                                if let ReturnType::Type(_, return_type) = &mut method.sig.output {
                                    if let Type::Path(type_path) = &mut **return_type {
                                        if let Some((_, const_value)) = get_struct_const_arg(type_path) {
                                            let new_expr = create_arithmetic_expr(&const_params, const_value);
                                            replace_const_arg(return_type, new_expr);
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
        "The mutation operator replaces constant values in generic return types with arithmetic expressions involving existing const parameters and modifies function calls to use inferred const parameters. This stresses the compiler's const generic evaluation and inference systems, potentially exposing bugs in type-checking and const propagation."
    }
}

struct MethodCallVisitor;

impl<'ast> VisitMut for MethodCallVisitor {
    fn visit_expr_method_call_mut(&mut self, i: &mut syn::ExprMethodCall) {
        if let Expr::Path(receiver_path) = &mut *i.receiver {
            if let Some(args) = get_receiver_generic_args(receiver_path) {
                replace_receiver_const_arg(args);
            }
        }
        visit_mut::visit_expr_method_call_mut(self, i);
    }
}

fn get_const_params(generics: &syn::Generics) -> Option<Vec<&syn::GenericParam>> {
    let const_params: Vec<_> = generics.params.iter()
        .filter(|param| matches!(param, syn::GenericParam::Const(_)))
        .collect();
    if const_params.is_empty() {
        None
    } else {
        Some(const_params)
    }
}

fn get_struct_const_arg(type_path: &syn::TypePath) -> Option<(String, u64)> {
    if type_path.qself.is_some() {
        return None;
    }
    let segments = &type_path.path.segments;
    if segments.len() != 1 {
        return None;
    }
    let segment = &segments[0];
    if let syn::PathArguments::AngleBracketed(args) = &segment.arguments {
        if args.args.len() != 1 {
            return None;
        }
        if let syn::GenericArgument::Const(const_arg) = &args.args[0] {
            if let Expr::Lit(expr_lit) = &*const_arg {
                if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                    if let Ok(value) = lit_int.base10_parse::<u64>() {
                        return Some((segment.ident.to_string(), value));
                    }
                }
            }
        }
    }
    None
}

fn create_arithmetic_expr(const_params: &[&syn::GenericParam], const_value: u64) -> Box<syn::Expr> {
    if let Some(const_param) = const_params.first() {
        if let syn::GenericParam::Const(const_param) = const_param {
            let ident = &const_param.ident;
            let plus_one = const_value + 1;
            let expr = parse_quote! { { #ident + #plus_one } };
            return Box::new(expr);
        }
    }
    parse_quote! { 0 }
}

fn replace_const_arg(return_type: &mut Box<syn::Type>, new_expr: Box<syn::Expr>) {
    if let syn::Type::Path(type_path) = &mut **return_type {
        let segments = &mut type_path.path.segments;
        if let Some(segment) = segments.last_mut() {
            if let syn::PathArguments::AngleBracketed(args) = &mut segment.arguments {
                if args.args.len() == 1 {
                    if let syn::GenericArgument::Const(const_arg) = args.args[0] {
                        *const_arg = *new_expr;
                    }
                }
            }
        }
    }
}

fn get_receiver_generic_args(receiver_path: &mut syn::ExprPath) -> Option<&mut syn::AngleBracketedGenericArguments> {
    let segments = &mut receiver_path.path.segments;
    if segments.len() != 1 {
        return None;
    }
    let segment = &mut segments[0];
    if let syn::PathArguments::AngleBracketed(args) = &mut segment.arguments {
        Some(args)
    } else {
        None
    }
}

fn replace_receiver_const_arg(args: &mut syn::AngleBracketedGenericArguments) {
    if args.args.len() == 1 {
        if let syn::GenericArgument::Const(const_arg) = args.args[0] {
            if let &Expr::Lit(expr_lit) = &*const_arg {
                if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                    let new_lit = syn::LitInt::new("0", lit_int.span());
                    let new_expr = syn::Expr::Lit(syn::ExprLit {
                        attrs: Vec::new(),
                        lit: syn::Lit::Int(new_lit),
                        ..expr_lit.clone()
                    });
                    *const_arg = new_expr;
                }
            }
        }
    }
}