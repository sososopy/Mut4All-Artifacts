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

pub struct Modify_Const_Generics_In_Function_Constraints_410;

impl Mutator for Modify_Const_Generics_In_Function_Constraints_410 {
    fn name(&self) -> &str {
        "Modify_Const_Generics_In_Function_Constraints_410"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let Some(where_clause) = &mut item_fn.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            for bound in &mut predicate_type.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(generic_param) = trait_bound
                                        .path
                                        .segments
                                        .first_mut()
                                        .and_then(|segment| segment.arguments.first_mut())
                                    {
                                        if let GenericArgument::Const(expr) = generic_param {
                                            if let Expr::Path(expr_path) = expr {
                                                if let Some(ident) = expr_path.path.get_ident() {
                                                    if ident == "usize" {
                                                        *ident = Ident::new("u8", ident.span());
                                                        if let Some(last_segment) = trait_bound
                                                            .path
                                                            .segments
                                                            .last_mut()
                                                        {
                                                            if let PathArguments::AngleBracketed(
                                                                angle_bracketed_args,
                                                            ) =
                                                                &mut last_segment.arguments
                                                            {
                                                                for arg in &mut angle_bracketed_args.args {
                                                                    if let GenericArgument::Const(
                                                                        expr,
                                                                    ) = arg
                                                                    {
                                                                        if let Expr::Path(expr_path) = expr {
                                                                            if let Some(ident) =
                                                                                expr_path.path.get_ident()
                                                                            {
                                                                                if ident == "C" {
                                                                                    *expr = parse_quote! { C as usize };
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
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "Identify functions with a where clause using const generics. Modify the const generic type specifier from usize to u8. Adjust the array in the trait bound to ensure it remains valid with the new type by casting C to usize."
    }
}