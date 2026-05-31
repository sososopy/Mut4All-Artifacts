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

pub struct Alter_Const_Trait_Bounds_32;

impl Mutator for Alter_Const_Trait_Bounds_32 {
    fn name(&self) -> &str {
        "Alter_Const_Trait_Bounds_32"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if item_impl.constness.is_some() {
                    if let Some((_, path, _)) = &item_impl.trait_ {
                        if path.segments.iter().any(|seg| seg.ident == "Deref" || seg.ident == "DerefMut") {
                            let mut rng = thread_rng();
                            let action: bool = rng.gen();
                            if action {
                                if let Some(gen) = &mut item_impl.generics.params.first_mut() {
                                    if let syn::GenericParam::Type(type_param) = gen {
                                        type_param.bounds.push(parse_quote!(Clone));
                                    }
                                }
                            } else {
                                if let Some(gen) = &mut item_impl.generics.params.first_mut() {
                                    if let syn::GenericParam::Type(type_param) = gen {
                                        type_param.bounds.clear();
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
        "The mutation operator targets const trait implementations of `Deref` and `DerefMut`. It either adds a `Clone` bound or removes any existing bounds from the type parameter. This exposes potential issues in the compiler's handling of const trait bounds, especially in terms of enforcement and error detection."
    }
}