use proc_macro2::{Span, Ident};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident as SynIdent,
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

pub struct Parameter_Count_Inconsistency_44;

impl Mutator for Parameter_Count_Inconsistency_44 {
    fn name(&self) -> &str {
        "Parameter_Count_Inconsistency_44"
    }
    fn mutate(&self, file: &mut syn::File) {
        let trait_methods = collect_trait_methods(file);
        let mut items = std::mem::take(&mut file.items);
        for item in &mut items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    for impl_item in &mut item_impl.items {
                        if let syn::ImplItem::Fn(method) = impl_item {
                            let trait_method_name = &method.sig.ident;
                            if let Some(trait_item) = find_trait_method(&trait_methods, path, trait_method_name) {
                                let impl_param_count = method.sig.inputs.len();
                                let trait_param_count = trait_item.sig.inputs.len();
                                
                                if impl_param_count != trait_param_count {
                                    if impl_param_count > trait_param_count {
                                        method.sig.inputs = method.sig.inputs.iter().take(trait_param_count).cloned().collect();
                                    } else {
                                        let additional_params = trait_param_count - impl_param_count;
                                        for _ in 0..additional_params {
                                            method.sig.inputs.push(parse_quote!(extra_param: i32));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        file.items = items;
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets trait implementations by ensuring that the number of parameters in the implementation methods either exceeds or falls short of the number specified in the trait declaration. It modifies the method signature by adding or removing parameters, which can lead to compilation errors and tests the compiler's ability to handle discrepancies between trait declarations and their implementations."
    }
}

fn collect_trait_methods(file: &syn::File) -> Vec<(&SynIdent, &syn::TraitItemFn)> {
    let mut trait_methods = Vec::new();
    for item in &file.items {
        if let syn::Item::Trait(trait_item) = item {
            for trait_item in &trait_item.items {
                if let syn::TraitItem::Fn(trait_method) = trait_item {
                    trait_methods.push((&trait_method.sig.ident, trait_method));
                }
            }
        }
    }
    trait_methods
}

fn find_trait_method<'a>(
    trait_methods: &'a Vec<(&SynIdent, &'a syn::TraitItemFn)>,
    trait_path: &syn::Path,
    method_name: &SynIdent,
) -> Option<&'a syn::TraitItemFn> {
    for (ident, trait_method) in trait_methods {
        if **ident == trait_path.segments.last().unwrap().ident {
            if &trait_method.sig.ident == method_name {
                return Some(trait_method);
            }
        }
    }
    None
}