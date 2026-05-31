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

pub struct Modify_Associated_Type_To_Recursive_Const_Bound_327;

impl Mutator for Modify_Associated_Type_To_Recursive_Const_Bound_327 {
    fn name(&self) -> &str {
        "Modify_Associated_Type_To_Recursive_Const_Bound_327"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                let mut has_recursive_bound = false;
                for assoc_item in &mut item_trait.items {
                    if let TraitItem::Type(assoc_type) = assoc_item {
                        for bound in &assoc_type.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound
                                    .path
                                    .segments
                                    .last()
                                    .map(|seg| seg.ident == item_trait.ident)
                                    .unwrap_or(false)
                                {
                                    has_recursive_bound = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                if has_recursive_bound {
                    item_trait.attrs.push(parse_quote!(#[const_trait]));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets trait definitions with associated types that have recursive trait bounds. It modifies such traits by adding the #[const_trait] attribute, transforming them into const traits. This change stresses the compiler's handling of const trait bounds and recursive type constraints, potentially exposing issues in trait resolution, const evaluation, and type inference."
    }
}