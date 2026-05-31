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

pub struct Lifetime_Elision_Disruption_In_Trait_Implementations_193;

impl Mutator for Lifetime_Elision_Disruption_In_Trait_Implementations_193 {
    fn name(&self) -> &str {
        "Lifetime_Elision_Disruption_In_Trait_Implementations_193"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, trait_path, _)) = &item_impl.trait_ {
                    let mut trait_segments = trait_path.segments.clone();
                    let last_segment = trait_segments.last_mut();
                    if let Some(last_segment) = last_segment {
                        match &last_segment.arguments {
                            PathArguments::AngleBracketed(args) => {
                                let mut has_lifetime_args = false;
                                for arg in &args.args {
                                    if let GenericArgument::Lifetime(_) = arg {
                                        has_lifetime_args = true;
                                        break;
                                    }
                                }
                                if has_lifetime_args {
                                    let mut new_args = args.clone();
                                    new_args.args.clear();
                                    for arg in &args.args {
                                        if let GenericArgument::Lifetime(_) = arg {
                                            continue;
                                        }
                                        new_args.args.push(arg.clone());
                                    }
                                    last_segment.arguments = PathArguments::AngleBracketed(new_args);
                                }
                            }
                            _ => {}
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait implementations where the trait definition includes explicit lifetime parameters. It removes lifetime arguments from the trait path in impl blocks, forcing the compiler to attempt lifetime elision. This creates mismatches between trait requirements and implementation, stressing compiler logic for lifetime elision in trait matching, especially with async functions and complex trait bounds."
    }
}