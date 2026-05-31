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

pub struct Add_Unused_Lifetime_Constraints_22;

impl Mutator for Add_Unused_Lifetime_Constraints_22 {
    fn name(&self) -> &str {
        "Add_Unused_Lifetime_Constraints_22"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let lifetimes = &mut func.sig.generics.params;
                let lifetime = Lifetime::new("'unused", Span::call_site());
                let lifetime_param = GenericParam::Lifetime(LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: lifetime.clone(),
                    colon_token: None,
                    bounds: Punctuated::new(),
                });
                lifetimes.push(lifetime_param);
                if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let syn::Type::Reference(ref mut reference) = **ty {
                        reference.lifetime = Some(lifetime);
                    }
                }
            }
            if let syn::Item::Struct(item_struct) = item {
                let lifetimes = &mut item_struct.generics.params;
                let lifetime = Lifetime::new("'unused", Span::call_site());
                let lifetime_param = GenericParam::Lifetime(LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: lifetime.clone(),
                    colon_token: None,
                    bounds: Punctuated::new(),
                });
                lifetimes.push(lifetime_param);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds an unused lifetime parameter `'unused` to function and struct signatures. This transformation introduces unnecessary lifetime constraints, potentially leading to ICEs or inference failures in the compiler's lifetime resolution system. It aims to test the robustness of the compiler's handling of lifetimes and their constraints."
    }
}