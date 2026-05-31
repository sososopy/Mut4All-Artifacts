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

pub struct Introduce_Nested_Impl_Trait_In_Return_Position_168;

impl Mutator for Introduce_Nested_Impl_Trait_In_Return_Position_168 {
    fn name(&self) -> &str {
        "Introduce_Nested_Impl_Trait_In_Return_Position_168"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_collector = TraitCollector::new();
        trait_collector.visit_file(&*file);
        let available_traits = trait_collector.traits;
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let return_type_ref = &item_fn.sig.output;
                if let ReturnType::Type(_, return_type) = return_type_ref {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        if let Some(first_bound) = type_impl_trait.bounds.first() {
                            if let TypeParamBound::Trait(trait_bound) = first_bound {
                                let trait_path = &trait_bound.path;
                                let trait_name = trait_path.segments.last().unwrap().ident.to_string();
                                let mut suitable_assoc_trait = None;
                                for available_trait_name in available_traits.iter() {
                                    if *available_trait_name != trait_name {
                                        suitable_assoc_trait = Some(available_trait_name.clone());
                                        break;
                                    }
                                }
                                if let Some(assoc_trait) = suitable_assoc_trait {
                                    let new_return_type: Type = parse_quote! {
                                        impl #trait_path<Item = impl #assoc_trait>
                                    };
                                    let new_body: Expr = parse_quote! {
                                        Box::new(()) as Box<dyn #trait_path<Item = impl #assoc_trait>>
                                    };
                                    let mut new_block = item_fn.block.clone();
                                    new_block.stmts.clear();
                                    new_block.stmts.push(Stmt::Expr(new_body