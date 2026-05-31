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

pub struct Modify_Trait_Alias_In_Const_Generics_99;

impl Mutator for Modify_Trait_Alias_In_Const_Generics_99 {
    fn name(&self) -> &str {
        "Modify_Trait_Alias_In_Const_Generics_99"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::TraitAlias(trait_alias) = item {
                if let Some(generic_args) = trait_alias
                    .generics
                    .make_where_clause()
                    .predicates
                    .iter()
                    .filter_map(|predicate| {
                        if let WherePredicate::Type(predicate_type) = predicate {
                            Some(&predicate_type.bounds)
                        } else {
                            None
                        }
                    })
                    .next()
                {
                    for arg in generic_args {
                        if let TypeParamBound::Trait(trait_bound) = arg {
                            for segment in &trait_bound.path.segments {
                                if let PathArguments::AngleBracketed(angle_bracketed_args) =
                                    &segment.arguments
                                {
                                    for arg in &angle_bracketed_args.args {
                                        if let GenericArgument::Const(expr) = arg {
                                            if let Expr::Binary(expr_binary) = expr {
                                                if let Expr::Lit(ExprLit {
                                                    lit: syn::Lit::Int(lit_int),
                                                    ..
                                                }) = &*expr_binary.right
                                                {
                                                    if let Ok(value) = lit_int.base10_parse::<i64>()
                                                    {
                                                        if value != 0 {
                                                            let new_expr: Expr =
                                                                parse_quote! { 6 / 2 };
                                                            *expr_binary = if expr_binary
                                                                .attrs
                                                                .is_empty()
                                                            {
                                                                ExprBinary {
                                                                    attrs: vec![],
                                                                    left: Box::new(new_expr),
                                                                    op: syn::BinOp::Div(
                                                                        syn::token::Div::default(),
                                                                    ),
                                                                    right: Box::new(
                                                                        Expr::Lit(ExprLit {
                                                                            attrs: vec![],
                                                                            lit: syn::Lit::Int(
                                                                                syn::LitInt::new(
                                                                                    "1",
                                                                                    Span::call_site(
                                                                                    ),
                                                                                ),
                                                                            ),
                                                                        }),
                                                                    ),
                                                                }
                                                            } else {
                                                                ExprBinary {
                                                                    attrs: expr_binary.attrs.clone(),
                                                                    left: Box::new(new_expr),
                                                                    op: expr_binary.op.clone(),
                                                                    right: expr_binary.right.clone(),
                                                                }
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
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets trait alias declarations that involve constant expressions within the associated types. It identifies the constant expression and replaces it with a division operation, ensuring the divisor is non-zero to maintain validity. By altering the constant expression in this way, the mutator aims to stress-test the compiler's handling of const generics and trait aliasing, potentially exposing weaknesses in const evaluation and type inference."
    }
}