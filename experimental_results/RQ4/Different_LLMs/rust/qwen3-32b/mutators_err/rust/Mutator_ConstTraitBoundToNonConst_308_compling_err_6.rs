use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, parse_quote,
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

pub struct Mutator_ConstTraitBoundToNonConst_308;

impl Mutator for Mutator_ConstTraitBoundToNonConst_308 {
    fn name(&self) -> &str {
        "Mutator_ConstTraitBoundToNonConst_308"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_traits = HashSet::new();
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for attr in &item_trait.attrs {
                    if attr.path().is_ident("const_trait") {
                        const_traits.insert(item_trait.ident.clone());
                        break;
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                for generic in &mut impl_item.generics.params {
                    if let GenericParam::Type(type_param) = generic {
                        for bound in &mut type_param.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if let Some(syn::Const(_)) = &trait_bound.modifier {
                                    if let Some(last_segment) = trait_bound.path.segments.last() {
                                        if const_traits.contains(&last_segment.ident) {
                                            let path_str = trait_bound.path.to_token_stream().to_string();
                                            let new_bound_str = format!("~const {}", path_str);
                                            let new_bound: TypeParamBound = syn::parse_str(&new_bound_str).unwrap();
                                            *bound = new_bound;
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
        "The mutation operator transforms const trait bounds in impl blocks to ~const, targeting traits marked with #[const_trait]. This forces the compiler to handle non-const trait usage in contexts expecting const correctness, potentially exposing ICEs during AST lowering or trait resolution."
    }
}