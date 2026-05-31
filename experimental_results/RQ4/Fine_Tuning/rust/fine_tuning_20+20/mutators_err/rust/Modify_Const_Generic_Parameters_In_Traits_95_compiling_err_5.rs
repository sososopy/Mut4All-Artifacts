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

pub struct Modify_Const_Generic_Parameters_In_Traits_95;

impl Mutator for Modify_Const_Generic_Parameters_In_Traits_95 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Parameters_In_Traits_95"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for assoc_item in &mut trait_item.items {
                    if let TraitItem::Const(assoc_const) = assoc_item {
                        if assoc_const.generics.params.is_empty() {
                            continue;
                        }
                        let mut new_expr = assoc_const.default.clone();
                        if let Some((_, Expr::Lit(expr_lit))) = &assoc_const.default {
                            if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                let value: i64 = lit_int.base10_parse().unwrap_or(0);
                                new_expr = Some((
                                    syn::token::Eq {
                                        spans: [lit_int.span()],
                                    },
                                    Expr::Binary(syn::ExprBinary {
                                        attrs: Vec::new(),
                                        left: Box::new(Expr::Lit(syn::ExprLit {
                                            attrs: Vec::new(),
                                            lit: syn::Lit::Int(syn::LitInt::new(
                                                &value.to_string(),
                                                lit_int.span(),
                                            )),
                                        })),
                                        op: syn::BinOp::Add(syn::token::Plus {
                                            spans: [lit_int.span()],
                                        }),
                                        right: Box::new(Expr::Lit(syn::ExprLit {
                                            attrs: Vec::new(),
                                            lit: syn::Lit::Int(syn::LitInt::new(
                                                "2",
                                                lit_int.span(),
                                            )),
                                        })),
                                    }),
                                ));
                            }
                        }
                        assoc_const.default = new_expr;
                    }
                }
            } else if let Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for pred in &mut func.sig.generics.where_clause.iter_mut().flat_map(
                    |where_clause| where_clause.predicates.iter_mut(),
                ) {
                    if let WherePredicate::Type(pred_ty) = pred {
                        if let Type::Path(type_path) = &pred_ty.bounded_ty {
                            if let Some(last_segment) = type_path.path.segments.last_mut() {
                                if last_segment.ident == "TraitWAssocConst" {
                                    if let PathArguments::AngleBracketed(args) =
                                        &mut last_segment.arguments
                                    {
                                        for arg in &mut args.args {
                                            if let GenericArgument::Const(expr) = arg {
                                                *expr = Expr::Binary(syn::ExprBinary {
                                                    attrs: Vec::new(),
                                                    left: Box::new(expr.clone()),
                                                    op: syn::BinOp::Add(syn::token::Plus {
                                                        spans: [expr.span()],
                                                    }),
                                                    right: Box::new(Expr::Lit(syn::ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Int(syn::LitInt::new(
                                                            "2",
                                                            expr.span(),
                                                        )),
                                                    })),
                                                });
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
        "This mutator targets traits with associated constants using const generics. It transforms simple constant expressions into more complex ones, such as arithmetic operations, and applies these modified constants in a generic context within a function. The goal is to stress-test the Rust compiler's handling of const generics and associated constants by introducing more intricate constant expressions, potentially uncovering issues in const evaluation and generic resolution."
    }
}