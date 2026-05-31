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

pub struct Parameter_Naming_Inconsistency_31;

impl Mutator for Parameter_Naming_Inconsistency_31 {
    fn name(&self) -> &str {
        "Parameter_Naming_Inconsistency_31"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut new_generics = HashSet::new();
                for param in &func.sig.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        let new_ident = Ident::new(&format!("{}_", type_param.ident), Span::call_site());
                        new_generics.insert((type_param.ident.clone(), new_ident.clone()));

                        // Replace usage within the function body
                        let mut visitor = TypeReplacer {
                            old_ident: type_param.ident.clone(),
                            new_ident: new_ident.clone(),
                        };
                        visitor.visit_block_mut(&mut func.block);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

struct TypeReplacer {
    old_ident: Ident,
    new_ident: Ident,
}

impl VisitMut for TypeReplacer {
    fn visit_type_mut(&mut self, node: &mut Type) {
        if let Type::Path(TypePath { path, .. }) = node {
            if path.segments.len() == 1 && path.segments[0].ident == self.old_ident {
                path.segments[0].ident = self.new_ident.clone();
            }
        }
        syn::visit_mut::visit_type_mut(self, node);
    }
}