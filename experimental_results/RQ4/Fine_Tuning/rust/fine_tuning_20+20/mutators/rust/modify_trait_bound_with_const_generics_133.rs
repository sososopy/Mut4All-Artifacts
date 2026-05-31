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

pub struct Modify_Trait_Bound_With_Const_Generics_133;

impl Mutator for Modify_Trait_Bound_With_Const_Generics_133 {
    fn name(&self) -> &str {
        "Modify_Trait_Bound_With_Const_Generics_133"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for item in &mut item_trait.items {
                    if let syn::TraitItem::Type(type_item) = item {
                        let mut has_const_generic_bound = false;
                        for bound in &type_item.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                for segment in &trait_bound.path.segments {
                                    if let PathArguments::AngleBracketed(args) =
                                        &segment.arguments
                                    {
                                        for arg in &args.args {
                                            if let GenericArgument::Const(_) = arg {
                                                has_const_generic_bound = true;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        if has_const_generic_bound {
                            for bound in &mut type_item.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    for segment in &mut trait_bound.path.segments {
                                        if let PathArguments::AngleBracketed(args) =
                                            &mut segment.arguments
                                        {
                                            for arg in &mut args.args {
                                                if let GenericArgument::Const(expr) = arg {
                                                    if let Expr::Lit(expr_lit) = expr {
                                                        if let syn::Lit::Int(lit_int) =
                                                            &expr_lit.lit
                                                        {
                                                            let original_value =
                                                                lit_int.base10_parse::<i32>();
                                                            if let Ok(value) = original_value {
                                                                let new_value = match value {
                                                                    1 => 0,
                                                                    0 => 2,
                                                                    _ => 1,
                                                                };
                                                                let new_lit: syn::Expr =
                                                                    parse_quote! { #new_value };
                                                                *expr = new_lit;
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
        "This mutator targets traits with associated types that have bounds using const generics. It modifies the const value in the bound's generic arguments, aiming to stress-test the compiler's handling of const generics in trait bounds by introducing potential logical changes or edge cases."
    }
}