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

pub struct Mutate_Const_Generic_Function_Impl_85;

impl Mutator for Mutate_Const_Generic_Function_Impl_85 {
    fn name(&self) -> &str {
        "Mutate_Const_Generic_Function_Impl_85"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if item_impl.generics.params.is_empty() {
                    continue;
                }
                let const_generic_indices: Vec<_> = item_impl
                    .generics
                    .params
                    .iter()
                    .enumerate()
                    .filter_map(|(i, param)| {
                        if matches!(param, GenericParam::Const(_)) {
                            Some(i)
                        } else {
                            None
                        }
                    })
                    .collect();
                if const_generic_indices.is_empty() {
                    continue;
                }
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let mut const_generic_used = false;
                        for i in &const_generic_indices {
                            if let Some(GenericParam::Const(const_param)) =
                                item_impl.generics.params.get(*i)
                            {
                                let const_ident = &const_param.ident;
                                if func
                                    .sig
                                    .inputs
                                    .iter()
                                    .any(|arg| match arg {
                                        FnArg::Typed(pat_type) => {
                                            pat_type
                                                .ty
                                                .to_token_stream()
                                                .to_string()
                                                .contains(&const_ident.to_string())
                                        }
                                        _ => false,
                                    })
                                    || func
                                        .sig
                                        .output
                                        .to_token_stream()
                                        .to_string()
                                        .contains(&const_ident.to_string())
                                {
                                    const_generic_used = true;
                                    break;
                                }
                            }
                        }
                        if !const_generic_used {
                            continue;
                        }
                        let mut new_stmts = Vec::new();
                        for stmt in &func.block.stmts {
                            new_stmts.push(stmt.clone());
                        }
                        let mut rng = thread_rng();
                        let mut mutated = false;
                        for i in &const_generic_indices {
                            if let Some(GenericParam::Const(const_param)) =
                                item_impl.generics.params.get(*i)
                            {
                                let const_ident = &const_param.ident;
                                if func
                                    .sig
                                    .inputs
                                    .iter()
                                    .any(|arg| match arg {
                                        FnArg::Typed(pat_type) => {
                                            pat_type
                                                .ty
                                                .to_token_stream()
                                                .to_string()
                                                .contains(&const_ident.to_string())
                                        }
                                        _ => false,
                                    })
                                    || func
                                        .sig
                                        .output
                                        .to_token_stream()
                                        .to_string()
                                        .contains(&const_ident.to_string())
                                {
                                    let new_expr: Expr = parse_quote! { #const_ident * 2 + 1 };
                                    let new_stmt = Stmt::Expr(new_expr, None);
                                    new_stmts.push(new_stmt);
                                    mutated = true;
                                }
                            }
                        }
                        if mutated {
                            func.block.stmts = new_stmts;
                        }
                        if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                            let mut used_const_generics = Vec::new();
                            for i in &const_generic_indices {
                                if let Some(GenericParam::Const(const_param)) =
                                    item_impl.generics.params.get(*i)
                                {
                                    let const_ident = &const_param.ident;
                                    if ty
                                        .to_token_stream()
                                        .to_string()
                                        .contains(&const_ident.to_string())
                                    {
                                        used_const_generics.push(const_ident.clone());
                                    }
                                }
                            }
                            if !used_const_generics.is_empty() {
                                let mut rng = thread_rng();
                                let new_expr = if used_const_generics.len() == 1 {
                                    let const_ident = &used_const_generics[0];
                                    if rng.gen_bool(0.5) {
                                        parse_quote! { #const_ident * 2 + 1 }
                                    } else {
                                        parse_quote! { #const_ident * 3 + 2 }
                                    }
                                } else {
                                    let const_ident_1 = &used_const_generics[0];
                                    let const_ident_2 = &used_const_generics[1];
                                    if rng.gen_bool(0.5) {
                                        parse_quote! { #const_ident_1 + #const_ident_2 }
                                    } else {
                                        parse_quote! { #const_ident_1 * #const_ident_2 }
                                    }
                                };
                                *ty = Box::new(syn::Type::Path(syn::TypePath {
                                    qself: None,
                                    path: syn::Path {
                                        leading_colon: None,
                                        segments: {
                                            let mut segs = Punctuated::new();
                                            segs.push(syn::PathSegment {
                                                ident: Ident::new("Combination", Span::call_site()),
                                                arguments: syn::PathArguments::AngleBracketed(
                                                    syn::AngleBracketedGenericArguments {
                                                        colon2_token: None,
                                                        lt_token: Default::default(),
                                                        args: {
                                                            let mut args = Punctuated::new();
                                                            args.push(syn::GenericArgument::Const(
                                                                syn::Expr::Group(syn::ExprGroup {
                                                                    group_token: Default::default(),
                                                                    expr: Box::new(new_expr),
                                                                }),
                                                            ));
                                                            args
                                                        },
                                                        gt_token: Default::default(),
                                                    },
                                                ),
                                            }));
                                            segs
                                        },
                                    },
                                }));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets function implementations within impl blocks that use const generics. It identifies functions where const generics are utilized in the signature, particularly in return types. The mutator then modifies the function body to include arithmetic expressions involving the const generics, aiming to create more complex const expressions that challenge the compiler's const evaluation logic. This transformation stresses the compiler's ability to handle const generic arithmetic and type inference, potentially revealing bugs in const propagation and evaluation."
    }
}