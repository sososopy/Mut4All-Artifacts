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

pub struct DuplicateConstParamCycle_142;

impl Mutator for DuplicateConstParamCycle_142 {
    fn name(&self) -> &str {
        "DuplicateConstParamCycle_142"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                let generics = &mut trait_item.generics;
                for (i, param) in generics.params.iter_mut().enumerate() {
                    if let syn::GenericParam::Const(original) = param {
                        let original_ident = original.ident.clone();
                        let new_param = syn::GenericParam::Const(syn::ConstParam {
                            attrs: vec![],
                            const_token: syn::token::Const::default(),
                            ident: original_ident.clone(),
                            colon_token: Some(syn::token::Colon::default()),
                            ty: Box::new(syn::parse_quote!(usize)),
                            eq_token: Some(syn::token::Eq::default()),
                            default: Some(Box::new(syn::parse_quote!(#original_ident))),
                        });
                        generics.params.insert(i + 1, new_param);
                        break;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}