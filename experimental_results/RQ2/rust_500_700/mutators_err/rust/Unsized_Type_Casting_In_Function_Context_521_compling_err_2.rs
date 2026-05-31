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

pub struct Unsized_Type_Casting_In_Function_Context_521;

impl Mutator for Unsized_Type_Casting_In_Function_Context_521 {
    fn name(&self) -> &str {
        "Unsized_Type_Casting_In_Function_Context_521"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(item_fn) = item {
                if let Some(cast_expr) = find_cast_expression(&item_fn.block.stmts) {
                    if let Some((fn_type, _)) = extract_fn_type_from_cast(cast_expr) {
                        let unsized_type: syn::Type = parse_quote!(str);
                        let new_fn_type = syn::TypeBareFn {
                            lifetimes: None,
                            unsafety: fn_type.unsafety,
                            abi: fn_type.abi.clone(),
                            fn_token: fn_type.fn_token,
                            paren_token: fn_type.paren_token,
                            inputs: fn_type.inputs.iter().map(|_| {
                                syn::BareFnArg {
                                    attrs: Vec::new(),
                                    name: None,
                                    ty: unsized_type.clone(),
                                }
                            }).collect(),
                            variadic: fn_type.variadic.clone(),
                            output: syn::ReturnType::Type(
                                token::RArrow { spans: [Span::call_site()] },
                                Box::new(unsized_type.clone()),
                            ),
                        };
                        *cast_expr = syn::Expr::Cast(syn::ExprCast {
                            attrs: Vec::new(),
                            expr: Box::new(syn::Expr::Path(syn::ExprPath {
                                attrs: Vec::new(),
                                qself: None,
                                path: SynPath::from(Ident::new("B", Span::call_site())),
                            })),
                            as_token: token::As {
                                span: Span::call_site(),
                            },
                            ty: Box::new(syn::Type::BareFn(new_fn_type)),
                        });
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies function type casting expressions and modifies them to include unsized types like `str` as parameters and return types. This transformation challenges the compiler's handling of unsized types in function contexts, potentially uncovering code generation issues or latent bugs in type resolution."
    }
}

fn find_cast_expression(stmts: &mut [Stmt]) -> Option<&mut Expr> {
    for stmt in stmts {
        if let Stmt::Local(Local { init: Some(LocalInit { expr, .. }), .. }) = stmt {
            if let Expr::Cast(_) = **expr {
                return Some(expr);
            }
        }
    }
    None
}

fn extract_fn_type_from_cast(expr: &Expr) -> Option<(&syn::TypeBareFn, &Expr)> {
    if let Expr::Cast(syn::ExprCast { expr, ty, .. }) = expr {
        if let syn::Type::BareFn(fn_type) = &**ty {
            return Some((fn_type, expr));
        }
    }
    None
}