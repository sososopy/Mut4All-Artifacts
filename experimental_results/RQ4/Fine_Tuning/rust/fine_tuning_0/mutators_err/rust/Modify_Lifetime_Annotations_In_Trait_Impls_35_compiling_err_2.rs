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

pub struct Modify_Lifetime_Annotations_In_Trait_Impls_35;

impl Mutator for Modify_Lifetime_Annotations_In_Trait_Impls_35 {
    fn name(&self) -> &str {
        "Modify_Lifetime_Annotations_In_Trait_Impls_35"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some((_, ref mut generics, _)) = item_impl.trait_ {
                    if let Some(lifetime_param) = generics.params.iter_mut().find_map(|param| {
                        if let syn::GenericParam::Lifetime(lifetime_param) = param {
                            Some(lifetime_param)
                        } else {
                            None
                        }
                    }) {
                        let original_lifetime = lifetime_param.lifetime.clone();
                        let new_lifetime = Lifetime::new("'b", Span::call_site());
                        lifetime_param.lifetime = new_lifetime.clone();

                        for impl_item in &mut item_impl.items {
                            if let syn::ImplItem::Fn(method) = impl_item {
                                for input in &mut method.sig.inputs {
                                    if let syn::FnArg::Typed(pat_type) = input {
                                        if let syn::Type::Reference(ref mut reference) = *pat_type.ty {
                                            if reference.lifetime.as_ref() == Some(&original_lifetime) {
                                                reference.lifetime = Some(new_lifetime.clone());
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
        ""
    }
}