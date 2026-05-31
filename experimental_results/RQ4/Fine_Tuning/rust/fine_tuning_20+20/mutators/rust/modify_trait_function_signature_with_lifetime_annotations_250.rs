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

pub struct Modify_Trait_Function_Signature_With_Lifetime_Annotations_250;

impl Mutator for Modify_Trait_Function_Signature_With_Lifetime_Annotations_250 {
    fn name(&self) -> &str {
        "Modify_Trait_Function_Signature_With_Lifetime_Annotations_250"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for item in &mut item_trait.items {
                    if let TraitItem::Fn(item_fn) = item {
                        let mut has_lifetime = false;
                        for param in &item_fn.sig.generics.params {
                            if let GenericParam::Lifetime(_) = param {
                                has_lifetime = true;
                                break;
                            }
                        }
                        if has_lifetime {
                            let mut new_generics = item_fn.sig.generics.clone();
                            new_generics.params.clear();
                            for param in &item_fn.sig.generics.params {
                                if let GenericParam::Lifetime(lifetime_def) = param {
                                    let mut new_lifetime_def = lifetime_def.clone();
                                    if new_lifetime_def.bounds.is_empty() {
                                        new_lifetime_def.bounds.push(lifetime_def.lifetime.clone());
                                    }
                                    new_generics
                                        .params
                                        .push(GenericParam::Lifetime(new_lifetime_def));
                                } else {
                                    new_generics.params.push(param.clone());
                                }
                            }
                            item_fn.sig.generics = new_generics;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets trait function signatures with lifetime annotations and adds a redundant self-referential lifetime constraint. By transforming lifetimes like `'a` into `'a: 'a`, it subtly alters the lifetime semantics, potentially exposing bugs in the compiler's lifetime resolution and trait system."
    }
}