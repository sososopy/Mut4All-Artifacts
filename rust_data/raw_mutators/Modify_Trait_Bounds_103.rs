use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemImpl, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Modify_Trait_Bounds_103;

impl Mutator for Modify_Trait_Bounds_103 {
    fn name(&self) -> &str {
        "Modify_Trait_Bounds_103"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(ItemImpl { generics, trait_, self_ty, .. }) = item {
                if let Some((_, path, _)) = trait_ {
                    if let Some(last_segment) = path.segments.last() {
                        if last_segment.ident == "Deserialize" {
                            for param in &mut generics.params {
                                if let GenericParam::Type(type_param) = param {
                                    for bound in &mut type_param.bounds {
                                        if let TypeParamBound::Trait(trait_bound) = bound {
                                            if trait_bound.path.is_ident("DeserializeOwned") {
                                                trait_bound.path.segments.push(parse_quote!(Clone));
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
        "This mutator targets generic implementations with trait bounds, specifically those implementing the Deserialize trait. It modifies the trait bounds by adding an additional Clone constraint to the existing DeserializeOwned trait bound. This tests the compiler's handling of additional constraints in generic contexts."
    }
}