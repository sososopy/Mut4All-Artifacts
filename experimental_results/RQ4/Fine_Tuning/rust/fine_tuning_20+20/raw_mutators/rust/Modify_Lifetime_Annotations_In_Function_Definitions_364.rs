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

pub struct Modify_Lifetime_Annotations_In_Function_Definitions_364;

impl Mutator for Modify_Lifetime_Annotations_In_Function_Definitions_364 {
    fn name(&self) -> &str {
        "Modify_Lifetime_Annotations_In_Function_Definitions_364"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut has_lifetime = false;
                for param in item_fn.sig.generics.params.iter() {
                    if matches!(param, GenericParam::Lifetime(_)) {
                        has_lifetime = true;
                        break;
                    }
                }
                if !has_lifetime {
                    continue;
                }
                let new_lifetime = Lifetime::new("'c", Span::call_site());
                item_fn
                    .sig
                    .generics
                    .params
                    .push(GenericParam::Lifetime(LifetimeParam::new(
                        new_lifetime.clone(),
                    )));
                for input in &mut item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Reference(type_reference) = &mut *pat_type.ty {
                            type_reference.lifetime = Some(new_lifetime.clone());
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "Identify functions with lifetime annotations, add an extra unused lifetime, and modify parameter references to use the new lifetime, potentially creating escaping bound variable scenarios."
    }
}