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

pub struct Modify_Trait_Bound_With_Const_Generics_286;

impl Mutator for Modify_Trait_Bound_With_Const_Generics_286 {
    fn name(&self) -> &str {
        "Modify_Trait_Bound_With_Const_Generics_286"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(ref mut trait_item) = item {
                for item in &mut trait_item.items {
                    if let syn::TraitItem::Type(ref mut type_item) = item {
                        if let Some(ref mut bounds) = type_item.bounds.first_mut() {
                            if let TypeParamBound::Trait(TraitBound { ref mut path, .. }) = bounds {
                                for segment in &mut path.segments {
                                    if let PathArguments::AngleBracketed(ref mut args) = segment.arguments {
                                        for arg in &mut args.args {
                                            if let GenericArgument::Const(Expr::Lit(ref mut lit)) = arg {
                                                if let syn::Lit::Int(ref mut int_lit) = lit.lit {
                                                    let current_value = int_lit.base10_parse::<i32>().unwrap_or(0);
                                                    let new_value = match current_value {
                                                        1 => 0,
                                                        _ => 1,
                                                    };
                                                    *int_lit = syn::LitInt::new(&new_value.to_string(), Span::call_site());
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
        "The mutation operator modifies trait declarations with associated type constraints that use const generics. It identifies traits with associated types that have bounds utilizing const generics and changes the const value in the generic parameters. This can potentially lead to logic changes or edge cases in compilation by using values like 0i32 or 2i32."
    }
}