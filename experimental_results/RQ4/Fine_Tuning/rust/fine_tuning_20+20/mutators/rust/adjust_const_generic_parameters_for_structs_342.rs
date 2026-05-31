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

pub struct Adjust_Const_Generic_Parameters_For_Structs_342;

impl Mutator for Adjust_Const_Generic_Parameters_For_Structs_342 {
    fn name(&self) -> &str {
        "Adjust_Const_Generic_Parameters_For_Structs_342"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut const_params = Vec::new();
                let mut new_generics = item_struct.generics.clone();
                for param in &item_struct.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        const_params.push(const_param.clone());
                    }
                }
                if const_params.is_empty() {
                    continue;
                }
                let mut new_const_params = Vec::new();
                for (i, const_param) in const_params.iter().enumerate() {
                    let new_ident = Ident::new(
                        &format!("A{}", i),
                        const_param.ident.span(),
                    );
                    let new_param = ConstParam {
                        attrs: const_param.attrs.clone(),
                        const_token: const_param.const_token,
                        ident: new_ident.clone(),
                        colon_token: const_param.colon_token,
                        ty: const_param.ty.clone(),
                        eq_token: None,
                        default: None,
                    };
                    new_const_params.push(new_param);
                }
                new_generics.params = new_generics
                    .params
                    .into_iter()
                    .filter(|param| !matches!(param, GenericParam::Const(_)))
                    .collect();
                for new_param in new_const_params.iter() {
                    new_generics.params.push(GenericParam::Const(new_param.clone()));
                }
                item_struct.generics = new_generics;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets struct definitions with const generic parameters. It renames these parameters to unique identifiers (A0, A1, etc.) and removes any default values or constraints that reference other const parameters. This transformation aims to eliminate cyclic dependencies and ambiguities in const generics, which can lead to internal compiler errors."
    }
}