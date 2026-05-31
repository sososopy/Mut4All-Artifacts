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

pub struct Modify_Lifetimes_In_Fn_Signatures_40;

impl Mutator for Modify_Lifetimes_In_Fn_Signatures_40 {
    fn name(&self) -> &str {
        "Modify_Lifetimes_In_Fn_Signatures_40"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let Some(bound_lifetimes) = &mut item_fn.sig.generics.params.iter_mut().find_map(|param| {
                    if let GenericParam::Lifetime(lifetime_param) = param {
                        Some(lifetime_param)
                    } else {
                        None
                    }
                }) {
                    bound_lifetimes.lifetime.ident = Ident::new("b", Span::call_site());
                }
                
                for input in &mut item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Reference(type_reference) = &mut *pat_type.ty {
                            if let Some(lifetime) = &mut type_reference.lifetime {
                                lifetime.ident = Ident::new("static", Span::call_site());
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies the lifetimes in function signatures, changing lifetime parameters to 'b' and reference lifetimes to 'static'. This transformation aims to introduce potential lifetime conflicts, testing the compiler's ability to handle complex lifetime relationships and higher-ranked trait bounds."
    }
}