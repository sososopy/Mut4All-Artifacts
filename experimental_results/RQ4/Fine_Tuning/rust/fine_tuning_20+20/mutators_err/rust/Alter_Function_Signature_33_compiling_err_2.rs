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

pub struct Alter_Function_Signature_33;

impl Mutator for Alter_Function_Signature_33 {
    fn name(&self) -> &str {
        "Alter_Function_Signature_33"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for item in &mut item_trait.items {
                    if let TraitItem::Type(type_item) = item {
                        for bound in &mut type_item.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound
                                    .path
                                    .segments
                                    .last()
                                    .map_or(false, |seg| seg.ident == "Deref")
                                {
                                    trait_bound.path = parse_quote!(DerefMut);
                                }
                            }
                        }
                    }
                }
            }
            if let Item::Impl(item_impl) = item {
                for item in &mut item_impl.items {
                    if let ImplItem::Type(type_item) = item {
                        for bound in &mut type_item.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound
                                    .path
                                    .segments
                                    .last()
                                    .map_or(false, |seg| seg.ident == "Deref")
                                {
                                    trait_bound.path = parse_quote!(DerefMut);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets associated types within traits and their corresponding impls. It searches for associated types bounded by the `Deref` trait and replaces this bound with `DerefMut`. By altering the trait bound, it challenges the compiler's trait resolution and coherence checks, potentially exposing bugs related to trait method dispatch and mutable reference handling."
    }
}