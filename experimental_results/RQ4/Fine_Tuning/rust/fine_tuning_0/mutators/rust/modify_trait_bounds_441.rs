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

pub struct Modify_Trait_Bounds_441;

impl Mutator for Modify_Trait_Bounds_441 {
    fn name(&self) -> &str {
        "Modify_Trait_Bounds_441"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(item_trait) = item {
                for supertrait in &mut item_trait.supertraits {
                    if let syn::TypeParamBound::Trait(trait_bound) = supertrait {
                        if trait_bound.lifetimes.is_some() {
                            continue;
                        }
                        trait_bound.lifetimes = Some(BoundLifetimes {
                            for_token: token::For {
                                span: Span::call_site(),
                            },
                            lt_token: token::Lt {
                                spans: [Span::call_site()],
                            },
                            gt_token: token::Gt {
                                spans: [Span::call_site()],
                            },
                            lifetimes: {
                                let mut lifetimes = Punctuated::new();
                                lifetimes.push(GenericParam::Lifetime(LifetimeParam::new(Lifetime::new("'a", Span::call_site()))));
                                lifetimes
                            },
                        });
                        trait_bound.path.segments.iter_mut().for_each(|segment| {
                            if let PathArguments::AngleBracketed(ref mut args) = segment.arguments {
                                args.args.insert(0, GenericArgument::Lifetime(Lifetime::new("'a", Span::call_site())));
                            }
                        });
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies trait bounds with higher-ranked trait bounds (HRTBs) by replacing `for<T>` with `for<'a>` and explicitly adding a lifetime parameter to the trait path. This transformation tests the compiler's handling of explicit lifetimes in HRTBs, potentially exposing bugs in lifetime management and variance."
    }
}