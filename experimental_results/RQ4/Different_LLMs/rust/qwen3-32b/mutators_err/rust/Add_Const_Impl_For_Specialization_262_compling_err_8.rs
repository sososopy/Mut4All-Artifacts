use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemImpl, ItemTrait, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Add_Const_Impl_For_Specialization_262;

impl Mutator for Add_Const_Impl_For_Specialization_262 {
    fn name(&self) -> &str {
        "Add_Const_Impl_For_Specialization_262"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut non_const_traits = HashSet::new();
        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                let has_const_trait = trait_item.attrs.iter().any(|attr| {
                    if let syn::Meta::Path(path) = &attr.meta {
                        path.is_ident("const_trait")
                    } else {
                        false
                    }
                });
                if !has_const_trait {
                    non_const_traits.insert(trait_item.ident.to_string());
                }
            }
        }

        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                if impl_item.defaultness.is_none() {
                    if let Some((_, trait_path, _)) = &mut impl_item.trait_ {
                        let trait_name = trait_path.segments.last().unwrap().ident.to_string();
                        if non_const_traits.contains(&trait_name) {
                            if let Some(const_token) = impl_item.constness {
                                // Already has const, skip
                            } else {
                                impl_item.constness = Some(token::Const::default());
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a 'const' qualifier to specialization impl blocks while ensuring the base trait is not marked with #[const_trait]. This creates a mismatch between const qualifiers and trait attributes, potentially exposing bugs in the compiler's handling of const trait coherence and constraint resolution during specialization."
    }
}