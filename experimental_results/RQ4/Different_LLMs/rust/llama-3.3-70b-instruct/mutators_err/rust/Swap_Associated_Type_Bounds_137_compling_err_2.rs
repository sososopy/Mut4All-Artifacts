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

pub struct Swap_Associated_Type_Bounds_137;

impl Mutator for Swap_Associated_Type_Bounds_137 {
    fn name(&self) -> &str {
        "Swap_Associated_Type_Bounds_137"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                let mut associated_types: Vec<(Ident, Punctuated<TypeParamBound, Comma>)> = trait_item
                    .generics
                    .type_params()
                    .map(|param| (param.ident.clone(), param.bounds.clone()))
                    .collect();

                if associated_types.len() > 1 {
                    let mut rng = thread_rng();
                    let idx1 = rng.gen_range(0..associated_types.len());
                    let idx2 = rng.gen_range(0..associated_types.len());

                    if idx1 != idx2 {
                        associated_types.swap(idx1, idx2);

                        let mut new_type_params = Punctuated::new();
                        for (ident, bounds) in associated_types {
                            new_type_params.push(GenericParam::Type(syn::TypeParam {
                                attrs: Vec::new(),
                                ident,
                                colon_token: Some(token::Colon::default()),
                                bounds,
                                default: None,
                                eq_token: None,
                                default_span: Span::call_site(),
                            }));
                        }

                        trait_item.generics.params.push(syn::GenericParam::Type(syn::TypeParam {
                            attrs: Vec::new(),
                            ident: Ident::new("dummy", Span::call_site()),
                            colon_token: Some(token::Colon::default()),
                            bounds: Punctuated::new(),
                            default: None,
                            eq_token: None,
                            default_span: Span::call_site(),
                        }));
                        trait_item.generics.params = new_type_params;
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator swaps the bounds of two associated types in a trait definition, introducing type inconsistencies and testing the compiler's ability to handle trait bounds and associated types."
    }
}