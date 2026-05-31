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

pub struct Modify_Dyn_Cast_Target_33;

impl Mutator for Modify_Dyn_Cast_Target_33 {
    fn name(&self) -> &str {
        "Modify_Dyn_Cast_Target_33"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::TraitObject(type_trait_object) = &mut **return_type {
                        let mut rng = thread_rng();
                        if type_trait_object.bounds.len() > 1 {
                            let mut bounds_vec: Vec<_> = type_trait_object.bounds.iter().collect();
                            bounds_vec.shuffle(&mut rng);
                            type_trait_object.bounds = Punctuated::from_iter(bounds_vec.into_iter().cloned());
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with `dyn*` return types. It randomly shuffles the trait bounds within `dyn*` casts, exploring different trait combinations. This transformation aims to test the compiler's handling of `dyn*` casts with varied trait orders, potentially revealing inconsistencies or issues in trait object representation and type checking."
    }
}