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

pub struct Add_Unused_Lifetimes_220;

impl Mutator for Add_Unused_Lifetimes_220 {
    fn name(&self) -> &str {
        "Add_Unused_Lifetimes_220"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let lifetimes = vec!["'a", "'b", "'c"];
                let mut rng = thread_rng();
                let num_lifetimes = rng.gen_range(1..=3);
                for _ in 0..num_lifetimes {
                    let lifetime_ident = Ident::new(lifetimes.choose(&mut rng).unwrap(), Span::call_site());
                    func.sig.generics.params.push(GenericParam::Lifetime(LifetimeParam {
                        attrs: Vec::new(),
                        lifetime: Lifetime {
                            apostrophe: Span::call_site(),
                            ident: lifetime_ident,
                        },
                        colon_token: None,
                        bounds: Punctuated::new(),
                    }));
                }
            }
            if let syn::Item::Struct(item_struct) = item {
                let lifetimes = vec!["'x", "'y", "'z"];
                let mut rng = thread_rng();
                let num_lifetimes = rng.gen_range(1..=3);
                for _ in 0..num_lifetimes {
                    let lifetime_ident = Ident::new(lifetimes.choose(&mut rng).unwrap(), Span::call_site());
                    item_struct.generics.params.push(GenericParam::Lifetime(LifetimeParam {
                        attrs: Vec::new(),
                        lifetime: Lifetime {
                            apostrophe: Span::call_site(),
                            ident: lifetime_ident,
                        },
                        colon_token: None,
                        bounds: Punctuated::new(),
                    }));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds randomly selected unused lifetimes to function and struct definitions. This transformation introduces unnecessary complexity into the lifetime system, potentially triggering compiler ICEs or inference issues related to lifetime resolution, unused lifetime warnings, or conflicts with existing lifetimes."
    }
}