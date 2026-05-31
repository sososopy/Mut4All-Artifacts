use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet,
    fs,
    ops::Range,
    panic,
    path::Path,
    process::Command,
};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    visit::Visit,
    visit_mut::VisitMut,
};

use crate::mutator::Mutator;

pub struct Duplicate_Const_Param_Name_In_Trait_83;

impl Mutator for Duplicate_Const_Param_Name_In_Trait_83 {
    fn name(&self) -> &str {
        "Duplicate_Const_Param_Name_In_Trait_83"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                let generics = &mut trait_item.generics;
                let mut first_const_idx = None;
                for (i, param) in generics.params.iter().enumerate() {
                    if let GenericParam::Const(_) = param {
                        first_const_idx = Some(i);
                        break;
                    }
                }
                if let Some(idx) = first_const_idx {
                    let mut params = generics.params.clone().into_iter().collect::<Vec<_>>();
                    let mut original_param = params.remove(idx);
                    if let GenericParam::Const(ref mut original_const) = original_param {
                        let ident = original_const.ident.clone();
                        let default_expr = Expr::Path(ExprPath {
                            attrs: Vec::new(),
                            qself: None,
                            path: parse_quote!(ident),
                        });
                        original_const.default = Some(default_expr);
                        let second_new = GenericParam::Const(original_const.clone());
                        let first_new = original_param;
                        params.insert(idx, first_new);
                        params.insert(idx + 1, second_new);
                        generics.params = Punctuated::from_iter(params);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets traits with const generics and duplicates the first const parameter, introducing a second parameter with the same name and a default value referencing the first. This creates ambiguous or conflicting generic parameters, testing the compiler's ability to resolve duplicate names and default values in trait definitions. It may trigger ICEs or incorrect type resolution when the same name is reused with differing constraints."
    }
}