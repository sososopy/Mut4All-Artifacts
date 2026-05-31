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

pub struct Modify_Trait_Method_Signature_With_Lifetime_490;

impl Mutator for Modify_Trait_Method_Signature_With_Lifetime_490 {
    fn name(&self) -> &str {
        "Modify_Trait_Method_Signature_With_Lifetime_490"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                let mut has_lifetime = false;
                for param in &trait_item.generics.params {
                    if let syn::GenericParam::Lifetime(_) = param {
                        has_lifetime = true;
                        break;
                    }
                }
                if !has_lifetime {
                    trait_item.generics.params.push(parse_quote!('a));
                }

                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Fn(method) = trait_item {
                        if method.sig.asyncness.is_some() {
                            if method.sig.generics.params.is_empty() {
                                method.sig.generics.params.push(parse_quote!('a));
                            }
                            for input in &mut method.sig.inputs {
                                if let FnArg::Typed(pat_type) = input {
                                    if let Type::Reference(type_ref) = &mut *pat_type.ty {
                                        if type_ref.lifetime.is_none() {
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
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async functions within trait definitions. It introduces or modifies lifetime annotations in the method signatures. This transformation ensures that lifetimes are consistently applied to both parameters and return types, challenging the compiler's handling of lifetime elision and async trait method resolution."
    }
}