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

pub struct MutualTraitMethodCycleInsertion_146;

impl Mutator for MutualTraitMethodCycleInsertion_146 {
    fn name(&self) -> &str {
        "MutualTraitMethodCycleInsertion_146"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut traits: Vec<&mut ItemTrait> = file.items.iter_mut().filter_map(|item| {
            if let Item::Trait(trait_) = item {
                Some(trait_)
            } else {
                None
            }
        }).collect();

        if traits.len() < 2 {
            return;
        }

        let t1 = &mut traits[0];
        let t2 = &mut traits[1];

        // Find first method in t1
        let mut t1_method = None;
        for item in &mut t1.items {
            if let syn::ItemTrait::ItemTrait(trait_item) = item {
                if let syn::TraitItem::Fn(method) = &mut trait_item {
                    if !method.sig.inputs.is_empty() {
                        t1_method = Some(method);
                        break;
                    }
                }
            }
        }

        // Find first method in t2
        let mut t2_method = None;
        for item in &mut t2.items {
            if let syn::ItemTrait::ItemTrait(trait_item) = item {
                if let syn::TraitItem::Fn(method) = &mut trait_item {
                    if !method.sig.inputs.is_empty() {
                        t2_method = Some(method);
                        break;
                    }
                }
            }
        }

        if let (Some(t1_method), Some(t2_method)) = (t1_method, t2_method) {
            // Modify t1_method's first parameter and return type to t2's name
            modify_method(t1_method, &t2.ident);
            // Modify t2_method's first parameter and return type to t1's name
            modify_method(t2_method, &t1.ident);
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator creates mutual dependencies between two traits by modifying their first methods. It replaces a parameter and return type in each method with the other trait's type, creating a circular reference. This challenges the compiler's ability to resolve recursive trait relationships and may expose bugs in trait coherence or type inference systems."
    }
}

fn modify_method(method: &mut syn::TraitItemFn, other_trait_ident: &syn::Ident) {
    // Replace first parameter's type
    for input in &mut method.sig.inputs {
        if let FnArg::Typed(pat_type) = input {
            *pat_type.ty = syn::Type::Path(syn::TypePath {
                qself: None,
                path: syn::Path::from(other_trait_ident.clone()),
            });
            break;
        }
    }

    // Replace return type
    if let syn::ReturnType::Type(_, ref mut ty) = method.sig.output {
        *ty = syn::Type::Path(syn::TypePath {
            qself: None,
            path: syn::Path::from(other_trait_ident.clone()),
        });
    }
}