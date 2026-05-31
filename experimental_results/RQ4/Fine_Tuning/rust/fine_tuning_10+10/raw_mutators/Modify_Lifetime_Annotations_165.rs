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

pub struct Modify_Lifetime_Annotations_165;

impl Mutator for Modify_Lifetime_Annotations_165 {
    fn name(&self) -> &str {
        "Modify_Lifetime_Annotations_165"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut lifetimes = item_fn.sig.generics.params.iter().filter_map(|param| {
                    if let GenericParam::Lifetime(lt) = param {
                        Some(lt.lifetime.clone())
                    } else {
                        None
                    }
                }).collect::<Vec<_>>();

                if !lifetimes.is_empty() {
                    // Introduce a new lifetime 'c
                    let new_lifetime = Lifetime::new("'c", Span::call_site());
                    lifetimes.push(new_lifetime.clone());

                    // Add the new lifetime to the function's generics
                    item_fn.sig.generics.params.push(GenericParam::Lifetime(
                        LifetimeParam::new(new_lifetime.clone())
                    ));

                    // Modify the parameter list to introduce a reference using the new lifetime
                    if let Some(FnArg::Typed(PatType { ty, .. })) = item_fn.sig.inputs.first_mut() {
                        if let Type::Reference(ref mut type_ref) = **ty {
                            type_ref.lifetime = Some(new_lifetime);
                        }
                    }
                }
            }
        }
    }
    
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function definitions with lifetime annotations. It introduces a new lifetime 'c and modifies the function's parameter list to include a reference using this new lifetime. This can potentially create complex interactions between lifetimes, leading to escaping bound variable situations. The mutation is designed to test the Rust compiler's handling of lifetime annotations and interactions."
    }
}