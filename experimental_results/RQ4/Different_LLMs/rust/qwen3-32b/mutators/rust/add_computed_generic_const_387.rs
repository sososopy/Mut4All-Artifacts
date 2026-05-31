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

pub struct Add_Computed_Generic_Const_387;

impl Mutator for Add_Computed_Generic_Const_387 {
    fn name(&self) -> &str {
        "Add_Computed_Generic_Const_387"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                if generics.params.iter().any(|p| matches!(p, GenericParam::Const(_))) && generics.where_clause.is_some() {
                    let const_params: Vec<&GenericParam> = generics.params.iter().filter(|p| matches!(p, GenericParam::Const(_))).collect();
                    if let Some(first_const) = const_params.first() {
                        if let GenericParam::Const(const_param) = first_const {
                            let existing_name = &const_param.ident;
                            let new_name = Ident::new("NP", Span::call_site());
                            let new_param = parse_quote!(const #new_name: usize = { #existing_name + 1 });
                            generics.params.push(new_param);
                        }
                    }
                }
            } else if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        let generics = &mut method.sig.generics;
                        if generics.params.iter().any(|p| matches!(p, GenericParam::Const(_))) && generics.where_clause.is_some() {
                            let const_params: Vec<&GenericParam> = generics.params.iter().filter(|p| matches!(p, GenericParam::Const(_))).collect();
                            if let Some(first_const) = const_params.first() {
                                if let GenericParam::Const(const_param) = first_const {
                                    let existing_name = &const_param.ident;
                                    let new_name = Ident::new("NP", Span::call_site());
                                    let new_param = parse_quote!(const #new_name: usize = { #existing_name + 1 });
                                    generics.params.push(new_param);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a new generic const parameter derived from an existing const parameter via a computed expression (e.g., N + 1). This transformation targets functions and methods with where clauses and existing const parameters, stressing the compiler's handling of computed const expressions in generic contexts. It aims to uncover bugs in const evaluation, trait resolution, and generic parameter inference when expressions are used to derive new constants."
    }
}