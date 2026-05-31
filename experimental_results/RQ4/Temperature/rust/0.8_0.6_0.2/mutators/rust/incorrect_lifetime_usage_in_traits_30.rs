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

pub struct Incorrect_Lifetime_Usage_In_Traits_30;

impl Mutator for Incorrect_Lifetime_Usage_In_Traits_30 {
    fn name(&self) -> &str {
        "Incorrect_Lifetime_Usage_In_Traits_30"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                let lifetime_param: LifetimeParam = parse_quote!('a);
                trait_item.generics.params.push(GenericParam::Lifetime(lifetime_param));
            }
            if let syn::Item::Fn(func_item) = item {
                for input in &mut func_item.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &mut *pat_type.ty {
                            if let Some(segment) = type_path.path.segments.last_mut() {
                                if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                    args.args.push(GenericArgument::Lifetime(parse_quote!('static)));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}