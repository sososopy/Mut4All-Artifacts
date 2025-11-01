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

pub struct Adjust_Const_Generic_Params_143;

impl Mutator for Adjust_Const_Generic_Params_143 {
    fn name(&self) -> &str {
        "Adjust_Const_Generic_Params_143"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(ItemStruct { generics, .. }) = item {
                let mut new_params = Punctuated::new();
                let mut counter = 0;
                for param in &generics.params {
                    if let GenericParam::Const(const_param) = param {
                        let new_ident = Ident::new(&format!("C{}", counter), const_param.ident.span());
                        counter += 1;
                        let mut new_const_param = const_param.clone();
                        new_const_param.ident = new_ident;
                        new_const_param.default = Some(parse_quote! { 0 });
                        new_params.push(GenericParam::Const(new_const_param));
                    } else {
                        new_params.push(param.clone());
                    }
                }
                generics.params = new_params;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets struct definitions with const generic parameters. It renames these parameters to unique identifiers and replaces any default values with constant expressions or numeric literals, avoiding references to other parameters. This prevents cyclic dependencies and ambiguities, reducing the risk of internal compiler errors."
    }
}