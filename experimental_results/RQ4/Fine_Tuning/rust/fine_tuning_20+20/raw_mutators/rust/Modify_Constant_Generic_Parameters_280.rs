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

pub struct Modify_Constant_Generic_Parameters_280;

impl Mutator for Modify_Constant_Generic_Parameters_280 {
    fn name(&self) -> &str {
        "Modify_Constant_Generic_Parameters_280"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                if let Some(generics) = trait_item.generics.params.iter().find_map(|param| {
                    if let syn::GenericParam::Const(const_param) = param {
                        Some(const_param)
                    } else {
                        None
                    }
                }) {
                    let mut new_generics = trait_item.generics.clone();
                    for param in &mut new_generics.params {
                        if let syn::GenericParam::Const(const_param) = param {
                            if const_param.ident == generics.ident {
                                const_param.ty = syn::Type::Path(syn::TypePath {
                                    qself: None,
                                    path: syn::Path::from(syn::Ident::new(
                                        "i32",
                                        proc_macro2::Span::call_site(),
                                    )),
                                });
                                const_param.default = Some(syn::Expr::Lit(syn::ExprLit {
                                    attrs: Vec::new(),
                                    lit: syn::Lit::Int(syn::LitInt::new(
                                        "-1",
                                        proc_macro2::Span::call_site(),
                                    )),
                                }));
                            }
                        }
                    }
                    trait_item.generics = new_generics;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets traits with constant generic parameters, specifically altering the type and default value of such parameters. By changing a constant generic from `usize` to `i32` and adjusting its default value, the mutator stresses the compiler's handling of constant generics in trait contexts, potentially revealing weaknesses in type checking, trait resolution, and const evaluation."
    }
}