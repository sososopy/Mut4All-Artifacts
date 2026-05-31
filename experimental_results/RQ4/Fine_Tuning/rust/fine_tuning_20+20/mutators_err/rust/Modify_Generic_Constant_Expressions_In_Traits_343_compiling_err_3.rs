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

pub struct Modify_Generic_Constant_Expressions_In_Traits_343;

impl Mutator for Modify_Generic_Constant_Expressions_In_Traits_343 {
    fn name(&self) -> &str {
        "Modify_Generic_Constant_Expressions_In_Traits_343"
    }
    fn mutate(&self, file: &mut syn::File) {
        let trait_items: Vec<_> = file
            .items
            .iter()
            .filter_map(|item| {
                if let Item::Trait(item_trait) = item {
                    Some(item_trait)
                } else {
                    None
                }
            })
            .collect();
        for item_trait in trait_items {
            if let Some(generic_param) = item_trait
                .generics
                .params
                .iter()
                .find(|param| matches!(param, GenericParam::Const(_)))
            {
                if let GenericParam::Const(const_param) = generic_param {
                    let const_ident = &const_param.ident;
                    let mut impl_items: Vec<_> = file
                        .items
                        .iter_mut()
                        .filter_map(|item| {
                            if let Item::Impl(item_impl) = item {
                                Some(item_impl)
                            } else {
                                None
                            }
                        })
                        .collect();
                    for item_impl in impl_items {
                        if let Some((_, trait_path, _)) = &mut item_impl.trait_ {
                            if let Some(last_segment) = trait_path.segments.last_mut() {
                                if last_segment.ident == item_trait.ident {
                                    if let PathArguments::AngleBracketed(
                                        angle_bracketed_args,
                                    ) = &mut last_segment.arguments
                                    {
                                        for arg in &mut angle_bracketed_args.args {
                                            if let GenericArgument::Const(expr) = arg {
                                                if let Expr::Path(expr_path) = expr {
                                                    if expr_path.path.is_ident(const_ident) {
                                                        *expr = parse_quote! { {2 + 2} };
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
        "This mutator targets traits with constant generic parameters and modifies their usage in impl blocks. It replaces the constant argument with a nested arithmetic expression that evaluates to the same value. This transformation stresses the compiler's constant evaluation logic and aims to uncover weaknesses in how constant expressions are resolved within generic contexts."
    }
}