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

pub struct Duplicate_Const_Parameter_In_Trait_Definition_270;

impl Mutator for Duplicate_Const_Parameter_In_Trait_Definition_270 {
    fn name(&self) -> &str {
        "Duplicate_Const_Parameter_In_Trait_Definition_270"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                let mut const_params = Vec::new();
                for param in &trait_item.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        const_params.push(const_param.clone());
                    }
                }
                if !const_params.is_empty() {
                    for const_param in const_params {
                        let const_ident = &const_param.ident;
                        let const_ty = &const_param.ty;
                        let duplicate_param: GenericParam = parse_quote! { const #const_ident: #const_ty = #const_ident };
                        trait_item.generics.params.push(duplicate_param);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets trait definitions that include const generic parameters. It duplicates the const parameter in both the parameter list and assigns it a default value equal to the original parameter. This mutation can lead to compilation errors due to the illegal reuse of a const parameter name, potentially triggering ICEs."
    }
}