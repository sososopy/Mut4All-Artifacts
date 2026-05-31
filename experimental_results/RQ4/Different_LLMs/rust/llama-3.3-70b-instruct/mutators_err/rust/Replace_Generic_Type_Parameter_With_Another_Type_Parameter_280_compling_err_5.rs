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

pub struct Replace_Generic_Type_Parameter_With_Another_Type_Parameter_280;

impl Mutator for Replace_Generic_Type_Parameter_With_Another_Type_Parameter_280 {
    fn name(&self) -> &str {
        "Replace_Generic_Type_Parameter_With_Another_Type_Parameter_280"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                if trait_item.generics.type_params().len() == 0 {
                    continue;
                }
                let generics = trait_item.generics.clone();
                let params: Vec<_> = generics.type_params().into_iter().collect();
                if params.len() > 1 {
                    let idx_to_replace = thread_rng().gen_range(0..params.len());
                    let replacement_idx = thread_rng().gen_range(0..params.len());
                    let mut new_params = generics.type_params().into_iter().collect::<Vec<_>>();
                    new_params[idx_to_replace] = params[replacement_idx].clone();
                    trait_item.generics = syn::Generics {
                        lt_token: generics.lt_token,
                        gt_token: generics.gt_token,
                        where_clause: generics.where_clause,
                    };
                    trait_item.generics.type_params = new_params.into_iter().collect();
                }
            } else if let syn::Item::Struct(struct_item) = item {
                if struct_item.generics.type_params().len() == 0 {
                    continue;
                }
                let generics = struct_item.generics.clone();
                let params: Vec<_> = generics.type_params().into_iter().collect();
                if params.len() > 1 {
                    let idx_to_replace = thread_rng().gen_range(0..params.len());
                    let replacement_idx = thread_rng().gen_range(0..params.len());
                    let mut new_params = generics.type_params().into_iter().collect::<Vec<_>>();
                    new_params[idx_to_replace] = params[replacement_idx].clone();
                    struct_item.generics = syn::Generics {
                        lt_token: generics.lt_token,
                        gt_token: generics.gt_token,
                        where_clause: generics.where_clause,
                    };
                    struct_item.generics.type_params = new_params.into_iter().collect();
                }
            } else if let syn::Item::Fn(func) = item {
                if func.sig.generics.type_params().len() == 0 {
                    continue;
                }
                let generics = func.sig.generics.clone();
                let params: Vec<_> = generics.type_params().into_iter().collect();
                if params.len() > 1 {
                    let idx_to_replace = thread_rng().gen_range(0..params.len());
                    let replacement_idx = thread_rng().gen_range(0..params.len());
                    let mut new_params = generics.type_params().into_iter().collect::<Vec<_>>();
                    new_params[idx_to_replace] = params[replacement_idx].clone();
                    func.sig.generics = syn::Generics {
                        lt_token: generics.lt_token,
                        gt_token: generics.gt_token,
                        where_clause: generics.where_clause,
                    };
                    func.sig.generics.type_params = new_params.into_iter().collect();
                }
            } else if let syn::Item::Impl(impl_item) = item {
                if impl_item.generics.type_params().len() == 0 {
                    continue;
                }
                let generics = impl_item.generics.clone();
                let params: Vec<_> = generics.type_params().into_iter().collect();
                if params.len() > 1 {
                    let idx_to_replace = thread_rng().gen_range(0..params.len());
                    let replacement_idx = thread_rng().gen_range(0..params.len());
                    let mut new_params = generics.type_params().into_iter().collect::<Vec<_>>();
                    new_params[idx_to_replace] = params[replacement_idx].clone();
                    impl_item.generics = syn::Generics {
                        lt_token: generics.lt_token,
                        gt_token: generics.gt_token,
                        where_clause: generics.where_clause,
                    };
                    impl_item.generics.type_params = new_params.into_iter().collect();
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces a generic type parameter with another type parameter in trait, struct, function, and impl definitions. This transformation tests the compiler's handling of generic types and their constraints, potentially revealing bugs related to type inference, type checking, and trait resolution."
    }
}