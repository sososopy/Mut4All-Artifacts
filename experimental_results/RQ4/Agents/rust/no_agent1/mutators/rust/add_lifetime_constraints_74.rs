use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
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
};

use crate::mutator::Mutator;

pub struct Add_Lifetime_Constraints_74;

impl Mutator for Add_Lifetime_Constraints_74 {
    fn name(&self) -> &str {
        "Add_Lifetime_Constraints_74"
    }

    fn chain_of_thought(&self) -> &str {
        "Adding lifetime constraints to function signatures"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let generics = &mut func.sig.generics;
                let lifetime_param = LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: Lifetime::new("'a", Span::call_site()),
                    colon_token: None,
                    bounds: Punctuated::new(),
                };
                generics.params.push(GenericParam::Lifetime(lifetime_param));

                if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let Type::Path(TypePath { path, .. }) = &mut **return_type {
                        path.segments.iter_mut().for_each(|segment| {
                            if let PathArguments::AngleBracketed(ref mut args) = segment.arguments {
                                args.args.push(GenericArgument::Lifetime(Lifetime::new("'a", Span::call_site())));
                            }
                        });
                    }
                }
            }
        }
    }
}