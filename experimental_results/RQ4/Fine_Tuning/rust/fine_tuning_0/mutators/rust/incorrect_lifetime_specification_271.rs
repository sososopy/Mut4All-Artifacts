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

pub struct Incorrect_Lifetime_Specification_271;

impl Mutator for Incorrect_Lifetime_Specification_271 {
    fn name(&self) -> &str {
        "Incorrect_Lifetime_Specification_271"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut has_lifetime = false;
                for generic_param in &mut func.sig.generics.params {
                    if let syn::GenericParam::Lifetime(lifetime_param) = generic_param {
                        lifetime_param.lifetime = syn::Lifetime::new("'_", Span::call_site());
                        has_lifetime = true;
                    }
                }

                if !has_lifetime {
                    func.sig.generics.params.push(parse_quote!('static));
                }

                if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let syn::Type::Reference(ref mut type_ref) = **ty {
                        type_ref.lifetime = Some(syn::Lifetime::new("'_", Span::call_site()));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets functions with generic parameters involving lifetimes. It replaces explicit lifetime annotations with anonymous lifetimes, potentially leading to inference issues. If no lifetime is present, it incorrectly adds a 'static lifetime, testing the compiler's handling of lifetime elision and inference."
    }
}