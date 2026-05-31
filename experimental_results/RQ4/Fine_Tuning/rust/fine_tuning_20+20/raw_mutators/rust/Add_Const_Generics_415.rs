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

pub struct Add_Const_Generics_415;

impl Mutator for Add_Const_Generics_415 {
    fn name(&self) -> &str {
        "Add_Const_Generics_415"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let mut has_const_generic = false;
                let mut const_generic_ident = None;
                if let Some(generics) = &item_struct.generics.params.first() {
                    if let syn::GenericParam::Const(const_param) = generics {
                        has_const_generic = true;
                        const_generic_ident = Some(const_param.ident.clone());
                    }
                }
                if has_const_generic {
                    let const_generic_ident = const_generic_ident.unwrap();
                    let fn_name = Ident::new(
                        &format!("use_{}", item_struct.ident),
                        Span::call_site(),
                    );
                    let fn_item: syn::ItemFn = parse_quote! {
                        fn #fn_name<const N: usize>() {
                            let instance = #const_generic_ident::<{ N + 1 }> {};
                        }
                    };
                    file.items.push(syn::Item::Fn(fn_item));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets struct definitions with const generic parameters, particularly those with default values derived from constants. It introduces a new function that leverages a const generic parameter to instantiate the struct with a computed value, thereby stressing the compiler's const evaluation and generic resolution mechanisms."
    }
}