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
    ItemTrait, TraitItem, TraitItemMethod,
};

use crate::mutator::Mutator;

pub struct Modify_Trait_Method_Signature_With_Lifetime_Annotations_303;

impl Mutator for Modify_Trait_Method_Signature_With_Lifetime_Annotations_303 {
    fn name(&self) -> &str {
        "Modify_Trait_Method_Signature_With_Lifetime_Annotations_303"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                let lifetime_param = LifetimeParam {
                    attrs: vec![],
                    lifetime: Lifetime::new("'a", Span::call_site()),
                    colon_token: None,
                    bounds: Punctuated::new(),
                };

                if trait_item.generics.params.iter().all(|param| match param {
                    GenericParam::Lifetime(lifetime) => lifetime.lifetime != lifetime_param.lifetime,
                    _ => true,
                }) {
                    trait_item.generics.params.push(GenericParam::Lifetime(lifetime_param));
                }

                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Method(method) = trait_item {
                        if method.sig.asyncness.is_some() {
                            for input in &mut method.sig.inputs {
                                if let FnArg::Typed(pat_type) = input {
                                    if let Type::Reference(ref mut type_ref) = *pat_type.ty {
                                        type_ref.lifetime = Some(Lifetime::new("'a", Span::call_site()));
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