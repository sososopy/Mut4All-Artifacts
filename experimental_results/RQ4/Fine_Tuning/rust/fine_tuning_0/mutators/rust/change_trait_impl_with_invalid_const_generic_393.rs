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

pub struct Change_Trait_Impl_With_Invalid_Const_Generic_393;

impl Mutator for Change_Trait_Impl_With_Invalid_Const_Generic_393 {
    fn name(&self) -> &str {
        "Change_Trait_Impl_With_Invalid_Const_Generic_393"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    for segment in &path.segments {
                        if let PathArguments::AngleBracketed(args) = &segment.arguments {
                            for arg in &args.args {
                                if let GenericArgument::Const(expr) = arg {
                                    if let Expr::Path(expr_path) = expr {
                                        if let Some(last_segment) = expr_path.path.segments.last() {
                                            if last_segment.ident == "usize" || last_segment.ident == "isize" || last_segment.ident == "char" {
                                                let new_generic: GenericParam = parse_quote!(const M: f64);
                                                if let Some(const_generic) = item_impl.generics.params.iter_mut().find(|param| {
                                                    if let GenericParam::Const(const_param) = param {
                                                        const_param.ident == last_segment.ident
                                                    } else {
                                                        false
                                                    }
                                                }) {
                                                    *const_generic = new_generic;
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
        "This mutation operator targets trait implementations using const generics, specifically altering the type of const generic parameters from valid types (usize, isize, char) to an invalid floating-point type (f64). This change is intended to introduce a type mismatch, testing the compiler's handling of invalid const generic types and potentially exposing bugs in type checking and trait implementation."
    }
}