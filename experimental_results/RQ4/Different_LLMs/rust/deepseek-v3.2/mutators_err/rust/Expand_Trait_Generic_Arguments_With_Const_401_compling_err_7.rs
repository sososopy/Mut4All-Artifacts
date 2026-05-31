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

pub struct Expand_Trait_Generic_Arguments_With_Const_401;

impl Mutator for Expand_Trait_Generic_Arguments_With_Const_401 {
    fn name(&self) -> &str {
        "Expand_Trait_Generic_Arguments_With_Const_401"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                let mut has_const_param = false;
                let mut const_params = Vec::new();
                for param in &item_impl.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        has_const_param = true;
                        const_params.push(const_param);
                    }
                }
                if !has_const_param {
                    continue;
                }
                if let Some((maybe_not, trait_path, for_token)) = &item_impl.trait_ {
                    if let Some(last_segment) = trait_path.segments.last() {
                        if let PathArguments::AngleBracketed(args) = &last_segment.arguments {
                            if args.args.len() == 1 {
                                if let Some(GenericArgument::Type(_)) = args.args.first() {
                                    if let Some(const_param) = const_params.first() {
                                        let const_expr: Expr = parse_quote!({ #const_param.ident });
                                        let mut new_args = args.args.clone();
                                        new_args.push(GenericArgument::Const(const_expr));
                                        let mut new_segments = trait_path.segments.clone();
                                        let last_idx = new_segments.len() - 1;
                                        new_segments[last_idx].arguments = PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                                            colon2_token: None,
                                            lt_token: token::Lt::default(),
                                            args: new_args,
                                            gt_token: token::Gt::default(),
                                        });
                                        item_impl.trait_.as_mut().unwrap().1.segments = new_segments;
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
        "The mutation operator targets impl blocks implementing traits with a single generic type parameter and at least one const generic parameter. It expands the trait's generic argument list by adding a const expression wrapping the first const parameter from the impl header. This creates a mismatch where the trait expects one argument but receives two, with the second being a const of potentially forbidden type (e.g., user-defined struct). This aims to trigger internal compiler errors during const inference combination and trait matching."
    }
}