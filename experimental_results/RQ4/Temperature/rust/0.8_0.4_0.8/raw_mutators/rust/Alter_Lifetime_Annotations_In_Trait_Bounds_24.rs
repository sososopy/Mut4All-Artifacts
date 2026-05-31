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

pub struct Alter_Lifetime_Annotations_In_Trait_Bounds_24;

impl Mutator for Alter_Lifetime_Annotations_In_Trait_Bounds_24 {
    fn name(&self) -> &str {
        "Alter_Lifetime_Annotations_In_Trait_Bounds_24"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            for bound in &mut predicate_type.bounds {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(lifetimes) = &mut trait_bound.lifetimes {
                                        lifetimes.lifetimes.push(parse_quote!('b));
                                    } else {
                                        trait_bound.lifetimes = Some(BoundLifetimes {
                                            for_token: token::For { span: Span::call_site() },
                                            lt_token: token::Lt { spans: [Span::call_site()] },
                                            lifetimes: {
                                                let mut lifetimes = Punctuated::new();
                                                lifetimes.push(parse_quote!('b));
                                                lifetimes
                                            },
                                            gt_token: token::Gt { spans: [Span::call_site()] },
                                        });
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
        "The mutation operator targets lifetime annotations in trait bounds within function signatures. It introduces an additional lifetime 'b in trait bounds, particularly in async functions. This alteration tests the compiler's ability to handle multiple lifetimes in trait constraints, ensuring it does not improperly trigger internal compiler errors when additional lifetimes are present in generic bounds, which can be especially tricky in async contexts."
    }
}